// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/system/result.hpp>

#include <string>
#include <vector>

namespace coal {

template <typename T>
using Result = boost::system::result<T>;

namespace http = boost::beast::http;

using HttpResponse = http::response<http::string_body>;

using Cookies = std::vector<std::string>;

}  // namespace coal
