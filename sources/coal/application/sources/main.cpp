// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <auth_client.h>
#include <fmt_custom_types.h>
#include <http_requests.h>
#include <websocket_client.h>

#include <spdlog/spdlog.h>
#include <boost/asio/steady_timer.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>
#include <boost/cobalt/promise.hpp>
#include <boost/cobalt/race.hpp>
#include <boost/cobalt/this_coro.hpp>
#include <glaze/glaze.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/filter.hpp>

#include <chrono>
#include <coroutine>
#include <map>
#include <string>
#include <vector>

namespace coal {

using namespace boost;

static auto GenerateUntilZero(int max) -> cobalt::generator<int> {
  int i = max;
  while (i > 0) co_yield i--;
  co_return i;
}

static auto FromSocket() -> cobalt::generator<int> {
  return GenerateUntilZero(10);
}

static auto DestoryAndClearHandles(
    std::vector<std::coroutine_handle<>>& handles) -> void {
  auto not_done = [](auto& h) noexcept { return !h.done(); };
  auto not_done_handles = handles | ranges::views::filter(not_done);
  ranges::for_each(not_done_handles, [](auto& h) { h.destroy(); });
  handles.clear();
}

struct HandlesRegistry {
  std::map<int, std::vector<std::coroutine_handle<>>> handles;

  auto RegisterHandle(std::coroutine_handle<> h, int value_of_interest)
      -> void {
    handles[value_of_interest].emplace_back(std::move(h));
  }

  ~HandlesRegistry() {
    for (auto& handle : handles) {
      DestoryAndClearHandles(handle.second);
    }
  }
};

struct SubscribableSocket {
  cobalt::generator<int> socket;
  HandlesRegistry registry;
};

struct WebsocketRequest {
  std::string websocket_url = {};
  std::string port = {};
  std::string message = {};
};

struct Configuration {
  std::string url = {};
  Credentials credentials = {};
  WebsocketRequest ws_request = {};
};

static auto OnValue(HandlesRegistry& registry, int value) {
  struct Awaitable {
    HandlesRegistry& registry;
    int value = 0;

    auto await_ready() -> bool { return false; }

    auto await_suspend(std::coroutine_handle<> h) -> void {
      spdlog::info("Suspended with value: {}", value);
      registry.RegisterHandle(h, value);
    }

    auto await_resume() -> void {}
  };
  return Awaitable{registry, value};
}

static auto HandleValue(HandlesRegistry& registry, int value)
    -> cobalt::promise<void> {
  spdlog::info("Before waiting for value: {}", value);
  co_await OnValue(registry, value);
  spdlog::info("After waiting for value: {}", value);
}

static auto HandleIncomingMessages(HandlesRegistry& registry) {
  return cobalt::race(HandleValue(registry, 5), HandleValue(registry, 1),
                      HandleValue(registry, 1), HandleValue(registry, 3));
}

static auto DistributeIncomingMessages(HandlesRegistry& registry,
                                       cobalt::generator<int>& socket)
    -> cobalt::task<void> {
  for (auto value = co_await socket; value != -1; value = co_await socket) {
    if (auto search = registry.handles.find(value);
        search != registry.handles.end()) {
      for (auto handle : search->second) {
        spdlog::info("Found handle waiting for value: {} resuming it", value);
        handle.resume();
      }
      DestoryAndClearHandles(search->second);
    }
  }
}

static auto DelayMs(size_t ms) -> cobalt::task<void> {
  asio::steady_timer timer{co_await cobalt::this_coro::executor,
                           std::chrono::milliseconds(ms)};
  co_await timer.async_wait(cobalt::use_op);
}

static auto SpeakWithDelay() -> cobalt::detached {
  spdlog::info("Speak with delay started");
  co_await DelayMs(3000);
  spdlog::info("Speak with delay finished after 3 sec.");
}

static auto TestHttpRequests() -> cobalt::task<void> {
  const auto [result_1, result_2] = co_await cobalt::join(
      SendHttpGetRequestTo("https://adventure.land/data.js"),
      SendHttpGetRequestTo("http://127.0.0.1:8083/data.js"));
  if (result_1.has_error()) {
    spdlog::error("Error occured while sending http request: {} bailing out",
                  result_1.error().message());
    co_return;
  }
  if (result_2.has_error()) {
    spdlog::error("Error occured while sending http request: {} bailing out",
                  result_2.error().message());
    co_return;
  }
  spdlog::info("Returned responses are {} and {} bytes long",
               result_1.value().body().size(), result_2.value().body().size());
  co_return;
}

static auto TestAuthTo(Configuration config) -> cobalt::task<void> {
  const auto result = co_await AuthTo(config.url, config.credentials);
  if (result.has_error()) {
    spdlog::error("Error occured while login attempt: {} bailing out",
                  result.error().message());
    co_return;
  }
  spdlog::info("Got UserAuthData id: {} token: {}", result.value().id,
               result.value().token);

  const auto servers_result =
      co_await ServersAndCharactersFrom(config.url, result.value());
  if (servers_result.has_error()) {
    spdlog::error("Error occured while getting servers and characters: {}",
                  servers_result.error().message());
    co_return;
  }
  spdlog::info("Got {} servers and {} characters information",
               servers_result.value().servers.size(),
               servers_result.value().characters.size());
}

static auto SetupSpdLog() noexcept -> void {
  spdlog::set_pattern("[%X.%f] [%7i] [%^%L%$] %v");
}

static auto Test() -> cobalt::task<void> {
  spdlog::info("Before distributing messages");

  SubscribableSocket socket1{FromSocket(), {}};
  co_await cobalt::race(
      DistributeIncomingMessages(socket1.registry, socket1.socket),
      HandleIncomingMessages(socket1.registry));

  spdlog::info("After distributing messages");
}

[[nodiscard]] static auto ConfigurationFromFile(const std::string& path)
    -> Configuration {
  Configuration config = {
      .url = "http://127.0.0.1:8083",
      .credentials = {.email = "test@test.com", .password = "123456789"},
      .ws_request = {.websocket_url = "websocket-echo.com",
                     .port = "80",
                     .message = "Hello world!"}};

  const auto read_error = glz::read_file_json(config, path, std::string{});
  if (read_error) {
    spdlog::warn("Can't get configuration file, will use default values");
    const auto write_error = glz::write_file_json<glz::opts{.prettify = true}>(
        config, path, std::string{});
    if (write_error) {
      spdlog::warn("Can't write default configuration file");
    }
  }
  return config;
}

static auto TestAuthConnectivity() -> cobalt::task<void> {
  const auto config = ConfigurationFromFile("./.config.json");
  co_await TestAuthTo(config);
}

static auto TestWebsocketConnectivity() -> cobalt::task<void> {
  spdlog::info("Testing websockets");
  const auto config = ConfigurationFromFile("./.config.json");
  const auto& ws = config.ws_request;
  co_await DoSession(ws.websocket_url, ws.port, ws.message);
}

}  // namespace coal

auto co_main(int, char**) -> boost::cobalt::main {
  using namespace coal;
  SetupSpdLog();
  SpeakWithDelay();
  co_await TestWebsocketConnectivity();
  co_await TestAuthConnectivity();
  co_await TestHttpRequests();
  co_await Test();
  co_return 0;
}

// auto ImagineHandling() -> cobalt::promise<something> {

// co_await async_write(socket, boost::asio::buffer(data, n));
//  co_await async_write(socket, SomeCommand);
//  auto event_data = co_await async_read(socket, "event_name");
//   or
//   auto event_data = co_await
//   ExpectEvent<EventType/OrEventName/OrEventEnum>(socket, timeout);
// }

// Example - teleport between maps
// [[nodiscard]] auto TransportTo(MapName map, SpawnId
// spawn_id) nothrow -> cobalt::promise<bool?error?> {
//
// }

// Usage
// auto TestTransportTo() -> cobalt::promise<void>{
// auto player = co_await ConnectPlayer(...);
// if (!co_await player.TransportTo("main", 1)) {
// handle error
// }
// co_await MoveTo()....
// }

// Idea 2.
// Socket is actually a generator of Events, on which i can iterate untill it is
// runout of events, meaning connection is lost. Probably i need to co_await on
// values then.
