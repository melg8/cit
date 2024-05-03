// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef AUTH_CLIENT_H
#define AUTH_CLIENT_H

#include <universal_declarations.h>

#include <boost/cobalt.hpp>

#include <string>

namespace coal {
namespace cobalt = boost::cobalt;

struct Credentials {
  std::string email;
  std::string password;
};

cobalt::promise<Result<HttpResponse>> CallApiMethod(std::string_view url_text,
                                                    std::string_view method,
                                                    std::string_view args,
                                                    Cookies cookies = {});

cobalt::promise<Result<HttpResponse>> LoginTo(std::string_view url_text,
                                              const Credentials& credentials);

}  // namespace coal

#endif  // AUTH_CLIENT_H
