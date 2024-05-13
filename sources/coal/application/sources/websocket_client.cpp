// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <websocket_client.h>

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

//------------------------------------------------------------------------------

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
    co_return;
  }

  // Set a timeout on the operation
  beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

  // Make the connection on the IP address we get from a lookup
  auto [err_2] = co_await beast::get_lowest_layer(ws).async_connect(
      *endpoints.begin(), nothrow_use_op);

  if (err_2) {
    co_return;
  }

  // Update the host_ string. This will provide the value of the
  // Host HTTP header during the WebSocket handshake.
  // See https://tools.ietf.org/html/rfc7230#section-5.4
  host += ':' + port;

  // Turn off the timeout on the tcp_stream, because
  // the websocket stream has its own timeout system.
  beast::get_lowest_layer(ws).expires_never();

  // Set suggested timeout settings for the websocket
  ws.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::client));

  // Set a decorator to change the User-Agent of the handshake
  ws.set_option(
      websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(
            http::field::user_agent,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
      }));

  // Perform the websocket handshake
  co_await ws.async_handshake(host, "/", nothrow_use_op);

  // Send the message
  co_await ws.async_write(net::buffer(std::string(text)), nothrow_use_op);

  // This buffer will hold the incoming message
  beast::flat_buffer buffer;

  // Read a message into our buffer
  co_await ws.async_read(buffer, nothrow_use_op);

  // Close the WebSocket connection
  co_await ws.async_close(websocket::close_code::normal, nothrow_use_op);

  // If we get here then the connection is closed gracefully

  // The make_printable() function helps print a ConstBufferSequence
  spdlog::info("Got response from websocket size: {}", buffer.data().size());
}

}  // namespace coal