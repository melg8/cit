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

#include <stdexcept>
#include <cassert>
#include <iostream>

namespace al {

namespace cobalt = boost::cobalt;
namespace beast = boost::beast;

using executor_type = cobalt::use_op_t::executor_with_default<cobalt::executor>;
using socket_type = typename boost::asio::ip::tcp::socket::rebind_executor<
    executor_type>::other;
using ssl_socket_type = boost::asio::ssl::stream<socket_type>;
using acceptor_type = typename boost::asio::ip::tcp::acceptor::rebind_executor<
    executor_type>::other;
using websocket_type = beast::websocket::stream<ssl_socket_type>;

static cobalt::promise<ssl_socket_type> Connect(ServerEndpoint server_endpoint,
                                        boost::asio::ssl::context &ctx) {
  boost::asio::ip::tcp::resolver resolve{cobalt::this_thread::get_executor()};
  auto endpoints = co_await resolve.async_resolve(
    server_endpoint.host, server_endpoint.port, cobalt::use_op);
  ssl_socket_type sock{cobalt::this_thread::get_executor(), ctx};

  if(!SSL_set_tlsext_host_name(sock.native_handle(), 
                               server_endpoint.host.data())) {
    std::cout << "SSL_set_tlsext_host_name failed, error code: "
              << ::ERR_get_error();
  }

  std::cout << "Connecting\n";
  assert(!endpoints.empty()); // TODO(melg): find lib alternative to assert.
  co_await sock.next_layer().async_connect(*endpoints.begin());
  std::cout << "Connected\n";
  std::cout << "Handshaking\n";
  co_await sock.async_handshake(boost::asio::ssl::stream_base::client);
  std::cout << "Hand shook\n";
  co_return sock;
}

static cobalt::task<void> SendHttpsRequestTo(ServerEndpoint server_endpoint) {
  boost::asio::ssl::context ctx{boost::asio::ssl::context::tls_client};
  auto conn = co_await Connect(server_endpoint, ctx);
  std::cout << "connected\n";
  beast::http::request<beast::http::empty_body> req{beast::http::verb::get, "/data.js", 11};
  req.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.set("Accept-Encoding", "identity");
  req.set("Connection", "keep-alive");
  req.set(beast::http::field::host, server_endpoint.host);
  co_await beast::http::async_write(conn, req, cobalt::use_op);

  beast::flat_buffer b;
  beast::http::response<beast::http::string_body> response;
  co_await beast::http::async_read(conn, b, response, cobalt::use_op);
  std::cout << "Responce result: " << response.result()
            << " reason: " << response.reason();
  std::cout << "Responce body: " << response.body();
}

static cobalt::promise<beast::tcp_stream> ConnectTcpStream(ServerEndpoint server_endpoint) {
  beast::tcp_stream stream(cobalt::this_thread::get_executor());
  boost::asio::ip::tcp::resolver resolve{cobalt::this_thread::get_executor()};
  const auto port = server_endpoint.port.empty() ? "https" : server_endpoint.port;
  const auto endpoints = co_await resolve.async_resolve(
      server_endpoint.host, port, cobalt::use_op);
  std::cout << "Connecting\n";
  co_await stream.async_connect(endpoints, cobalt::use_op);
  std::cout << "Connected\n";
  co_return stream;
}

static cobalt::task<void> SendWithTcpHttpRequestTo(ServerEndpoint server_endpoint) {
  auto conn = co_await ConnectTcpStream(server_endpoint);
  std::cout << "Sending request\n";
  beast::http::request<beast::http::empty_body> req{beast::http::verb::get, "/data.js", 11};
  req.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.set("Accept-Encoding", "identity");
  req.set("Connection", "keep-alive");
  req.set(beast::http::field::host, server_endpoint.host);
  co_await beast::http::async_write(conn, req, cobalt::use_op);

  beast::flat_buffer b;
  beast::http::response<beast::http::string_body> response;
  co_await beast::http::async_read(conn, b, response, cobalt::use_op);
  std::cout << "Responce result: " << response.result()
            << " reason: " << response.reason();
  std::cout << "Responce body: " << response.body();
}

cobalt::task<void> SendHttpRequestTo(
    ServerEndpoint server_endpoint) {
  if (server_endpoint.port == "https") {
    return SendHttpsRequestTo(server_endpoint);
  } else {
    return SendWithTcpHttpRequestTo(server_endpoint);
  }
}

} // namespace al