// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <universal_declarations.h>

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/cobalt/task.hpp>
#include <string>

namespace coal {
//

struct WebsocketServer {
  std::string addr = {};
  std::string port = {};
};

using tcp = boost::asio::ip::tcp;
namespace cobalt = boost::cobalt;

auto DoSession(std::string host, std::string port, std::string text)
    -> cobalt::task<void>;

}  // namespace coal

#endif  // WEBSOCKET_CLIENT_H
