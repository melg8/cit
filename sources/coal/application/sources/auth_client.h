// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef AUTH_CLIENT_H
#define AUTH_CLIENT_H

#include <pretty_json_from_any_struct.h>
#include <servers_and_characters_response_from_json.h>
#include <universal_declarations.h>

#include <fmt/format.h>
#include <boost/cobalt.hpp>

#include <string>
#include <utility>

namespace coal {
namespace cobalt = boost::cobalt;

struct Credentials {
  std::string email = {};
  std::string password = {};
};

struct UserAuthData {
  std::string id = {};
  std::string token = {};
};

auto CallApiMethod(std::string_view url_text,
                   std::string_view method,
                   std::string_view args,
                   Cookies cookies = {})
    -> cobalt::promise<Result<HttpResponse>>;

auto AuthTo(std::string_view server_url, const Credentials& credentials)
    -> cobalt::promise<Result<UserAuthData>>;

auto GetServersAndCharacters(std::string_view url_text,
                             UserAuthData user_auth_data)
    -> cobalt::promise<Result<ServersAndCharactersResponse>>;

}  // namespace coal

namespace fmt {
template <>
struct formatter<coal::Credentials> : formatter<std::string> {
  auto format(const coal::Credentials& c, format_context& ctx) {
    return formatter<std::string>::format(JsonFrom(c), ctx);
  }
};

}  // namespace fmt

#endif  // AUTH_CLIENT_H
