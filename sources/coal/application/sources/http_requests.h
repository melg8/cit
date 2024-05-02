// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef HTTP_REQUESTS_H
#define HTTP_REQUESTS_H

#include <boost/cobalt.hpp>
#include <boost/system/result.hpp>

#include <string>
#include <string_view>

namespace coal {

namespace cobalt = boost::cobalt;

template <typename T>
using Result = boost::system::result<T>;

/**
 * @brief Send a GET request.
 * @param url_text text of server url that you want to send request.
 * @return result of request, system::error or std::string with data.
 */
cobalt::promise<Result<std::string>> SendHttpGetRequestTo(
    std::string_view url_text);

}  // namespace coal

#endif  // HTTP_REQUESTS_H