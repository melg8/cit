// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <auth_client.h>

#include <fmt_custom_types.h>
#include <http_requests.h>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <ctre.hpp>

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

[[nodiscard]] static Result<UserAuthData> FromCookie(std::string_view cookie) {
  const auto m = ctre::match<R"(.*auth=(.*?)-(.*?);.*)">(cookie);
  if (m) {
    return UserAuthData{.id = m.get<1>().to_string(),
                        .token = m.get<2>().to_string()};
  } else {
    spdlog::error("Can't find auth cookie in test: {}", cookie);
    return std::make_error_code(std::errc::protocol_error);
  };
}

[[nodiscard]] static Result<UserAuthData> FromResponse(
    const HttpResponse& repsonse) noexcept {
  const auto header = repsonse.base();
  for (const auto& field : header) {
    const auto& field_name = field.name_string();
    if (field_name == "set-cookie") {
      const auto& cookie = field.value();
      spdlog::info("Got set-cookie: {}", cookie);
      return FromCookie(cookie);
    }
  }
  spdlog::error("Can't find cookie in header of response: {}", header);
  return std::make_error_code(std::errc::protocol_error);
}

cobalt::promise<Result<UserAuthData>> AuthTo(std::string_view server_url,
                                             const Credentials& credentials) {
  const auto maybe_answer = co_await CallApiMethod(
      server_url, "signup_or_login", FormatOnlyLoginData(credentials));
  if (maybe_answer.has_error()) {
    co_return maybe_answer.error();
  }
  const auto answer = maybe_answer.value();
  const auto body = answer.body();
  if (!body.contains("Logged In!")) {
    spdlog::error("Can't login, server response: {}", body);
    co_return Result<UserAuthData>{
        std::make_error_code(std::errc::permission_denied)};
  }
  co_return FromResponse(answer);
}

}  // namespace coal