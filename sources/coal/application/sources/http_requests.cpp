// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <http_requests.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <boost/cobalt/race.hpp>
#include <boost/cobalt/this_thread.hpp>
#include <boost/beast.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/websocket/stream.hpp>

#include <spdlog/spdlog.h>
#include <libassert/assert.hpp>

#include <stdexcept>

namespace coal {

namespace cobalt = boost::cobalt;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = boost::asio::ip;

using executor_type = cobalt::use_op_t::executor_with_default<cobalt::executor>;
using socket_type = typename ip::tcp::socket::rebind_executor<
    executor_type>::other;
using ssl_socket_type = boost::asio::ssl::stream<socket_type>;
using acceptor_type = typename ip::tcp::acceptor::rebind_executor<
    executor_type>::other;
using websocket_type = beast::websocket::stream<ssl_socket_type>;

static void ReportError(boost::system::error_code err, 
  std::string_view action, ServerEndpoint server_endpoint) noexcept {
  DEBUG_ASSERT(err, "should not report errors with empty error code");
  spdlog::error("Error: {} while {}: {} port: {}", 
  err.message(), action, server_endpoint.host, server_endpoint.port);
}

static http::request<http::empty_body> FormGetRequestFor(
  ServerEndpoint server_endpoint, std::string_view target) {
  http::request<http::empty_body> req{http::verb::get,
                                                    target, 11};
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.set("Accept-Encoding", "identity");
  req.set("Connection", "keep-alive");
  req.set(http::field::host, server_endpoint.host);
  return req;
}

static cobalt::promise<Result<ssl_socket_type>> Connect(ServerEndpoint server_endpoint,
                                                        boost::asio::ssl::context &ctx) {
  ip::tcp::resolver resolve{cobalt::this_thread::get_executor()};
   const auto[err, endpoints] = co_await resolve.async_resolve(
    server_endpoint.host, server_endpoint.port, boost::asio::as_tuple(cobalt::use_op));
  if (err) {
    ReportError(err, "resolving host", server_endpoint);
    co_return err;
  }

  ssl_socket_type sock{cobalt::this_thread::get_executor(), ctx};
  if(!SSL_set_tlsext_host_name(sock.native_handle(), 
                               server_endpoint.host.data())) {
    const auto error_code = ::ERR_get_error();
    spdlog::error("SSL_set_tlsext_host_name failed, error code: {}",
                  error_code);
    co_return Result<ssl_socket_type>{
      std::error_code(error_code, std::generic_category())};
  }

  spdlog::info("Connecting");
  DEBUG_ASSERT(!endpoints.empty(), "endpoints must not be empty");
  const auto [connect_err] = co_await sock.next_layer()
      .async_connect(*endpoints.begin(), boost::asio::as_tuple(cobalt::use_op));
  if (connect_err) {
    ReportError(connect_err, "establishing connection to", server_endpoint);
  }
  spdlog::info("Connection success");

  spdlog::info("Handshaking");
  co_await sock.async_handshake(boost::asio::ssl::stream_base::client);
  spdlog::info("Handshake success");

  co_return Result<ssl_socket_type>{std::move(sock)};
}

static void LogResponce(const 
  http::response<http::string_body> &response,
  ServerEndpoint server_endpoint, std::string_view target) noexcept {
  const auto result = static_cast<int>(response.result());
  std::string reason{response.reason()};
  spdlog::info("Got http responce {} reason: {} body size {} from {}:{}{}",
  reason, result,response.body().size(),
  server_endpoint.host, server_endpoint.port, target);
}

static cobalt::promise<Result<void>> SendHttpsRequestTo(ServerEndpoint server_endpoint,
                                             std::string_view target) {
  boost::asio::ssl::context ctx{boost::asio::ssl::context::tls_client};
  auto conn = co_await Connect(server_endpoint, ctx);
  if (conn.has_error()) {
    ReportError(conn.error(), "connecting to endpoint host", server_endpoint);
    co_return conn.error();
  }

  spdlog::info("Sending \"get\" request");
  const auto request = FormGetRequestFor(server_endpoint, target);
  co_await http::async_write(*conn, request, cobalt::use_op);

  beast::flat_buffer b;
  http::response<http::string_body> response;
  co_await http::async_read(*conn, b, response, cobalt::use_op);
  LogResponce(response, server_endpoint, target);
  co_return {};
}

static cobalt::promise<Result<beast::tcp_stream>>
  ConnectTcpStream(ServerEndpoint server_endpoint) {
  beast::tcp_stream stream(cobalt::this_thread::get_executor());
  ip::tcp::resolver resolve{cobalt::this_thread::get_executor()};
  const auto port = server_endpoint.port.empty() ? "https" : server_endpoint.port;
  const auto[err, endpoints] = co_await resolve.async_resolve(
      server_endpoint.host, port, boost::asio::as_tuple(cobalt::use_op));
  if (err) {
    ReportError(err, "resolving host", server_endpoint);
    co_return err;
  }
  spdlog::info("Connecting");
  co_await stream.async_connect(endpoints, cobalt::use_op);
  spdlog::info("Connection success");
  co_return Result<beast::tcp_stream>{std::move(stream)};
}

static cobalt::promise<Result<void>> SendWithTcpHttpRequestTo(ServerEndpoint server_endpoint,
                                                   std::string_view target) {
  auto conn = co_await ConnectTcpStream(server_endpoint);
  if (conn.has_error()) {
    ReportError(conn.error(), "connecting to endpoint host", server_endpoint);
    co_return conn.error();
  }

  spdlog::info("Sending \"get\" request");
  const auto request = FormGetRequestFor(server_endpoint, target);
  co_await http::async_write(*conn, request, cobalt::use_op);

  beast::flat_buffer b;
  http::response<http::string_body> response;
  co_await http::async_read(*conn, b, response, cobalt::use_op);
  LogResponce(response, server_endpoint, target);
  co_return {};
}

cobalt::promise<Result<void>> SendHttpRequestTo(
    ServerEndpoint server_endpoint, std::string_view target) {
  spdlog::info("Sending request to host {} using \"{}\"", server_endpoint.host,
               server_endpoint.port);
  if (server_endpoint.port == "https") {
    return SendHttpsRequestTo(server_endpoint, target);
  } else {
    return SendWithTcpHttpRequestTo(server_endpoint, target);
  }
}

} // namespace coal