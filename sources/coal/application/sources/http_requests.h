// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef HTTP_REQUESTS_H
#define HTTP_REQUESTS_H

#include <boost/cobalt.hpp>
#include <boost/system/result.hpp>

#include <string_view>
#include <string>

namespace coal {

namespace cobalt = boost::cobalt;

template <typename T>
using Result = boost::system::result<T>;

/// Note: Formatted function depends on host/port order.
struct ServerEndpoint {
    std::string_view host;
    std::string_view port;
};

cobalt::promise<Result<std::string>> SendHttpRequestTo(
    ServerEndpoint server_endpoint,
    std::string_view target);

}  // namespace coal

#endif // HTTP_REQUESTS_H