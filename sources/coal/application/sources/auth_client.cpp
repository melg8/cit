// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <auth_client.h>

#include <http_requests.h>

#include <fmt/format.h>

namespace coal {

cobalt::promise<Result<HttpResponse>> CallApiMethod(std::string_view url_text,
                                                    std::string_view method,
                                                    std::string_view args,
                                                    Cookies cookies) {
  const auto api_url = fmt::format("{}/api/{}", url_text, method);
  return SendHttpPostRequestTo(api_url, method, args, cookies);
}

[[nodiscard]] static std::string FormatOnlyLoginData(
    const Credentials& credentials) {
  return fmt::format(
      "{{\"email\":\"{}\",\"password\":\"{}\",\"only_login\":true}}",
      credentials.email, credentials.password);
}

cobalt::promise<Result<HttpResponse>> LoginTo(std::string_view url_text,
                                              const Credentials& credentials) {
  return CallApiMethod(url_text, "signup_or_login",
                       FormatOnlyLoginData(credentials));
}

}  // namespace coal