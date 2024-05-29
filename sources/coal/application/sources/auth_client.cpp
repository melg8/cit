// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <auth_client.h>

#include <fmt_custom_types.h>
#include <http_requests.h>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <ctre.hpp>
#include <glaze/glaze.hpp>

#include <cctype>

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

[[nodiscard]] static Result<UserAuthData> FromCookie(std::string_view cookie) {
  const auto match = ctre::match<R"(.*auth=(.*?)-(.*?);.*)">(cookie);
  if (match) {
    return UserAuthData{.id = match.get<1>().to_string(),
                        .token = match.get<2>().to_string()};
  } else {
    spdlog::error("Can't find auth cookie in test: {}", cookie);
    return std::make_error_code(std::errc::protocol_error);
  }
}

[[nodiscard]] static Result<UserAuthData> FromResponse(
    const HttpResponse& repsonse) noexcept {
  const auto header = repsonse.base();
  for (const auto& field : header) {
    const auto& field_name = field.name_string();
    if (field_name == "Set-Cookie" || field_name == "set-cookie") {
      const auto& cookie = field.value();
      return FromCookie(cookie);
    }
  }
  spdlog::error("Can't find cookie in header of response: {}", header);
  return std::make_error_code(std::errc::protocol_error);
}

cobalt::promise<Result<UserAuthData>> AuthTo(std::string_view server_url,
                                             const Credentials& credentials) {
  const auto maybe_response = co_await CallApiMethod(
      server_url, "signup_or_login", FormatOnlyLoginData(credentials));
  if (maybe_response.has_error()) {
    co_return maybe_response.error();
  }
  const auto response = maybe_response.value();
  const auto body = response.body();
  if (body.find("Logged In!") == std::string::npos) {
    spdlog::error("Can't login, with credentials: {} server response: {}",
                  credentials, body);
    co_return Result<UserAuthData>{
        std::make_error_code(std::errc::permission_denied)};
  }
  co_return FromResponse(response);
}

[[nodiscard]] static std::string AuthCookieFrom(
    const UserAuthData& user_auth_data) {
  return fmt::format("auth={}-{}", user_auth_data.id, user_auth_data.token);
}

[[nodiscard]] static Result<ServersAndCharactersResponse>
ServersAndCharactersFrom(std::string_view json_body) {
  if (json_body.empty()) {
    spdlog::error("Got empty json body for servers and characters parsing");
    return std::make_error_code(std::errc::invalid_argument);
  }
  spdlog::info("Got json_body anwer: {}", json_body);
  const auto response =
      glz::read_json<ServersAndCharactersResponse>(ReducedFrom(json_body));
  if (!response) {
    spdlog::error("Failed to parse servers and characters json: {}",
                  ReducedFrom(json_body));
    return std::make_error_code(std::errc::bad_message);
  }
  spdlog::info("Servers and characters json parse succeeded");
  return response.value();
}

cobalt::promise<Result<ServersAndCharactersResponse>> GetServersAndCharacters(
    std::string_view url_text, UserAuthData user_auth_data) {
  const auto auth_cookie = AuthCookieFrom(user_auth_data);
  Cookies cookies = {auth_cookie};
  const auto maybe_answer =
      co_await CallApiMethod(url_text, "servers_and_characters", "{}", cookies);
  if (maybe_answer.has_error()) {
    spdlog::error("Can't obtains servers and characters: {}",
                  maybe_answer.error().message());
    co_return maybe_answer.error();
  }
  co_return ServersAndCharactersFrom(maybe_answer.value().body());
}

}  // namespace coal
