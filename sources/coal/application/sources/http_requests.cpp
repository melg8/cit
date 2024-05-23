// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <http_requests.h>

#include <universal_declarations.h>

#include <fmt_custom_types.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <boost/cobalt/this_thread.hpp>
#include <boost/url.hpp>

#include <fmt/color.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <libassert/assert.hpp>
#include <magic_enum/magic_enum.hpp>

namespace coal {

namespace cobalt = boost::cobalt;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace ip = net::ip;

using boost::urls::url_view;

using executor_type = cobalt::use_op_t::executor_with_default<cobalt::executor>;
using socket_type =
    typename ip::tcp::socket::rebind_executor<executor_type>::other;
using ssl_socket_type = ssl::stream<socket_type>;
using acceptor_type =
    typename ip::tcp::acceptor::rebind_executor<executor_type>::other;
using websocket_type = beast::websocket::stream<ssl_socket_type>;

using HttpRequest = http::request<http::string_body>;

static constexpr auto nothrow_use_op =
    boost::asio::as_tuple(boost::cobalt::use_op);

static void ReportError(boost::system::error_code err,
                        std::string_view action,
                        boost::urls::url url) noexcept {
  DEBUG_ASSERT(err, "should not report errors with empty error code");
  spdlog::error("Error: {} while {}: {}", err.message(), action, url);
}

[[nodiscard]] static std::string StringFrom(const Cookies& cookies) noexcept {
  std::string result{};
  for (const auto& cookie : cookies) {
    // TODO(melg): check if final cookie okay with ";" at the end.
    result += cookie + "; ";
  }
  return result;
}

static HttpRequest FormRequestFor(boost::urls::url url,
                                  http::verb verb,
                                  std::string_view body,
                                  Cookies cookies) {
  http::request<http::string_body> req{verb, url.path(), 11};
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.set("Accept-Encoding", "identity");
  req.set("Connection", "keep-alive");
  req.set(http::field::host, url.host());
  if (!body.empty()) {
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.body() = body;
  }
  if (!cookies.empty()) {
    req.set(http::field::cookie, StringFrom(cookies));
  }
  req.prepare_payload();
  return req;
}

static cobalt::promise<Result<ssl_socket_type>> Connect(boost::urls::url url,
                                                        ssl::context& ctx) {
  ip::tcp::resolver resolve{co_await cobalt::this_coro::executor};
  const auto port = url.port().empty() ? url.scheme() : url.port();
  const auto [err, endpoints] =
      co_await resolve.async_resolve(url.host(), port, nothrow_use_op);
  if (err) {
    ReportError(err, "resolving host", url);
    co_return err;
  }

  ssl_socket_type sock{co_await cobalt::this_coro::executor, ctx};
  if (!SSL_set_tlsext_host_name(sock.native_handle(), url.host().data())) {
    const auto error_code = ::ERR_get_error();
    spdlog::error("SSL_set_tlsext_host_name failed, error code: {}",
                  error_code);
    co_return Result<ssl_socket_type>{
        std::error_code(error_code, std::generic_category())};
  }

  spdlog::info("Connecting");
  DEBUG_ASSERT(!endpoints.empty(), "endpoints must not be empty");
  const auto [connect_err] = co_await sock.next_layer().async_connect(
      *endpoints.begin(), nothrow_use_op);
  if (connect_err) {
    ReportError(connect_err, "establishing connection to", url);
  }
  spdlog::info("Connection success");

  spdlog::info("Handshaking");
  co_await sock.async_handshake(ssl::stream_base::client);
  spdlog::info("Handshake success");

  co_return Result<ssl_socket_type>{std::move(sock)};
}

static void LogResponse(const HttpResponse& response,
                        boost::urls::url url) noexcept {
  const auto result = static_cast<int>(response.result());
  std::string reason{response.reason()};
  spdlog::info("Got http response {} reason: {} body size: {} from {}", reason,
               result, response.body().size(), url);
}

template <typename T>
static cobalt::promise<Result<HttpResponse>> SendUnifiedRequestTo(
    T& conn, boost::urls::url url, const HttpRequest& request) noexcept {
  spdlog::info("Sending request");
  const auto [write_err, _1] =
      co_await http::async_write(conn, request, nothrow_use_op);
  if (write_err) {
    ReportError(write_err, "writing to", url);
    co_return write_err;
  }

  beast::flat_buffer b;
  HttpResponse response;
  const auto [read_err, _2] =
      co_await http::async_read(conn, b, response, nothrow_use_op);
  if (read_err) {
    ReportError(read_err, "reading data from", url);
    co_return read_err;
  }
  LogResponse(response, url);
  co_return response;
}

static cobalt::promise<Result<HttpResponse>> SendHttpsRequestTo(
    boost::urls::url url, const HttpRequest& request) {
  ssl::context ctx{ssl::context::tls_client};
  auto conn = co_await Connect(url, ctx);
  if (conn.has_error()) {
    ReportError(conn.error(), "connecting to endpoint host", url);
    co_return conn.error();
  }
  co_return co_await SendUnifiedRequestTo(*conn, url, request);
}

static cobalt::promise<Result<beast::tcp_stream>> ConnectTcpStream(
    boost::urls::url url) {
  beast::tcp_stream stream(co_await cobalt::this_coro::executor);
  ip::tcp::resolver resolve{co_await cobalt::this_coro::executor};
  const auto port = url.port().empty() ? url.scheme() : url.port();
  const auto [err, endpoints] =
      co_await resolve.async_resolve(url.host(), port, nothrow_use_op);
  if (err) {
    ReportError(err, "resolving host", url);
    co_return err;
  }
  spdlog::info("Connecting");
  const auto [connect_err, _] =
      co_await stream.async_connect(endpoints, nothrow_use_op);
  if (connect_err) {
    ReportError(connect_err, "connection to", url);
    co_return connect_err;
  }
  spdlog::info("Connection success");
  co_return Result<beast::tcp_stream>{std::move(stream)};
}

static cobalt::promise<Result<HttpResponse>> SendWithTcpHttpRequestTo(
    boost::urls::url url, const HttpRequest& request) {
  auto conn = co_await ConnectTcpStream(url);
  if (conn.has_error()) {
    ReportError(conn.error(), "connecting to endpoint host", url);
    co_return conn.error();
  }
  co_return co_await SendUnifiedRequestTo(*conn, url, request);
}

static cobalt::promise<Result<HttpResponse>> SendHttpRequestTo(
    std::string_view url_text,
    http::verb verb,
    std::string_view body = {},
    const Cookies& cookies = {}) {
  const auto parsed_url = boost::urls::parse_uri(url_text);
  if (parsed_url.has_error()) {
    spdlog::error("Error while parsing url {}: error text: {}", url_text,
                  parsed_url.error().message());
    co_return Result<HttpResponse>{parsed_url.error()};
  }
  const boost::urls::url url = parsed_url.value();
  const auto request = FormRequestFor(url, verb, body, cookies);

  spdlog::info("Sending {} request to {}", magic_enum::enum_name(verb),
               url_text);
  if (url.scheme() == "https") {
    co_return co_await SendHttpsRequestTo(url, request);
  } else {
    co_return co_await SendWithTcpHttpRequestTo(url, request);
  }
}

cobalt::promise<Result<HttpResponse>> SendHttpGetRequestTo(
    std::string_view url_text) {
  return SendHttpRequestTo(url_text, http::verb::get);
}

[[nodiscard]] static std::string FormatBodyOfRequest(std::string_view method,
                                                     std::string_view args) {
  return fmt::format("method={}&arguments={}", method, args);
}

cobalt::promise<Result<HttpResponse>> SendHttpPostRequestTo(
    std::string_view url_text,
    std::string_view method,
    std::string_view args,
    const Cookies& cookies) {
  return SendHttpRequestTo(url_text, http::verb::post,
                           FormatBodyOfRequest(method, args), cookies);
}

}  // namespace coal