// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <websocket_client.h>

#include <universal_declarations.h>

#include <spdlog/spdlog.h>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>

namespace coal {

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

static constexpr auto nothrow_use_op =
    boost::asio::as_tuple(boost::cobalt::use_op);

class Stream {
 public:
  virtual ~Stream() = default;

  virtual cobalt::task<bool> AsyncRead(beast::flat_buffer&) = 0;
  virtual cobalt::task<bool> AsyncWrite(const net::const_buffer&) = 0;
  virtual cobalt::task<bool> AsyncClose() = 0;
};

class WebsocketOverTcpStream : public Stream {
 public:
  WebsocketOverTcpStream(websocket::stream<beast::tcp_stream>&& stream)
      : stream_(std::move(stream)) {}

  virtual cobalt::task<bool> AsyncRead(beast::flat_buffer& buffer) {
    const auto [err, _1] = co_await stream_.async_read(buffer, nothrow_use_op);
    co_return !err;
  }

  virtual cobalt::task<bool> AsyncWrite(const net::const_buffer& buffer) {
    const auto [err, _1] = co_await stream_.async_write(buffer, nothrow_use_op);
    co_return !err;
  }

  virtual cobalt::task<bool> AsyncClose() {
    const auto [err] = co_await stream_.async_close(
        websocket::close_code::normal, nothrow_use_op);
    co_return !err;
  }

  websocket::stream<beast::tcp_stream> stream_;
};

[[nodiscard]] static cobalt::task<void> UseStream(Stream&& stream,
                                                  std::string host,
                                                  std::string text) {
  spdlog::info("Sending message to websocket: {}", text);
  if (!co_await stream.AsyncWrite(net::buffer(text))) {
    spdlog::error("Error writing into web socket {}", host);
    co_return;
  }

  beast::flat_buffer buffer;
  if (!co_await stream.AsyncRead(buffer)) {
    spdlog::error("Error reading from web socket {}", host);
    co_return;
  }
  spdlog::info("Got response from websocket size: {}", buffer.data().size());

  if (!co_await stream.AsyncClose()) {
    spdlog::error("Can't close gracefully connection to {}", host);
    co_return;
  }
  spdlog::info("Websocket connection closed gracefuly with {}", host);
}

// Sends a WebSocket message and prints the response
cobalt::task<void> DoSession(std::string host,
                             std::string port,
                             std::string text) {
  // These objects perform our I/O
  auto resolver = tcp::resolver(co_await net::this_coro::executor);
  auto ws =
      websocket::stream<beast::tcp_stream>(co_await net::this_coro::executor);
  // Look up the domain name

  const auto [err_1, endpoints] =
      co_await resolver.async_resolve(host, port, nothrow_use_op);
  if (err_1) {
    spdlog::error("Can't resolve host {}:{}", host, port);
    co_return;
  }

  // Set a timeout on the operation
  beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

  // Make the connection on the IP address we get from a lookup
  auto [err_2] = co_await beast::get_lowest_layer(ws).async_connect(
      *endpoints.begin(), nothrow_use_op);

  if (err_2) {
    spdlog::error("Can't connect to {}:{}", host, port);
    co_return;
  }

  host += ':' + port;

  beast::get_lowest_layer(ws).expires_never();

  ws.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::client));

  ws.set_option(
      websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(
            http::field::user_agent,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
      }));

  const auto [handshake_err] =
      co_await ws.async_handshake(host, "/", nothrow_use_op);
  if (handshake_err) {
    spdlog::error("Handshake error with {} reason: {}", host,
                  handshake_err.message());
    co_return;
  }

  co_await UseStream(WebsocketOverTcpStream(std::move(ws)), host, text);
}

}  // namespace coal