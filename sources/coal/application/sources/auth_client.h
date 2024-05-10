// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef AUTH_CLIENT_H
#define AUTH_CLIENT_H

#include <universal_declarations.h>
#include <servers_and_characters_response_parser.h>

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

cobalt::promise<Result<HttpResponse>> CallApiMethod(std::string_view url_text,
                                                    std::string_view method,
                                                    std::string_view args,
                                                    Cookies cookies = {});

cobalt::promise<Result<UserAuthData>> AuthTo(std::string_view server_url,
                                             const Credentials& credentials);

cobalt::promise<Result<ServersAndCharactersResponse>> GetServersAndCharacters(
    std::string_view url_text, UserAuthData user_auth_data);

}  // namespace coal

#endif  // AUTH_CLIENT_H
