// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef HTTP_REQUESTS_H
#define HTTP_REQUESTS_H

#include <universal_declarations.h>

#include <boost/cobalt.hpp>

#include <string>
#include <string_view>

namespace coal {

namespace cobalt = boost::cobalt;

/**
 * @brief Send a GET request.
 * @param url_text text of server url that you want to send request.
 * @return result of request, system::error or HttpResponse with data.
 */
cobalt::promise<Result<HttpResponse>> SendHttpGetRequestTo(
    std::string_view url_text);

cobalt::promise<Result<HttpResponse>> SendHttpPostRequestTo(
    std::string_view url_text,
    std::string_view method,
    std::string_view args,
    const Cookies& cookies);

}  // namespace coal

#endif  // HTTP_REQUESTS_H
