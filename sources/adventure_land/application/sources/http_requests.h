// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef HTTP_REQUESTS_H
#define HTTP_REQUESTS_H

#include <boost/cobalt.hpp>

#include <string_view>

namespace al {

struct ServerEndpoint {
    std::string_view host;
    std::string_view port;
};

boost::cobalt::task<void> SendHttpRequestTo(
    ServerEndpoint server_endpoint);

} // namespace al;

#endif // HTTP_REQUESTS_H