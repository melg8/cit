// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <boost/cobalt/race.hpp>
#include <boost/cobalt/this_thread.hpp>
#include <boost/cobalt/this_coro.hpp>
#include <boost/cobalt/main.hpp>

#include <boost/asio/steady_timer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

#include <map>
#include <vector>
#include <coroutine>
#include <stdexcept>
#include <iostream>

#include <http_requests.h>

using namespace boost;

static cobalt::generator<int> test(int max) {
  int i = max;
  while (i > 0)
    co_yield i--;
  co_return i;
}

static cobalt::generator<int> FromSocket() {
  return test(10);
}

struct HandlesRegistry {
  std::map<int, std::vector<std::coroutine_handle<>>> handles;

  void RegisterHandle(std::coroutine_handle<> h, int value_of_interest) {
    handles[value_of_interest].push_back(h);
  }

  ~HandlesRegistry() {
    for (auto &handle: handles) {
      for (auto& subscibed : handle.second) {
        if (!subscibed.done()) {
          subscibed.destroy();
        }
      }
      handle.second.clear();
    }
  }
};

struct SubscribableSocket {
  cobalt::generator<int> socket;
  HandlesRegistry registry; // TODO(melg): split to repeatable and single shot registry.
};

static auto OnValue(HandlesRegistry &registry, int value) {

  struct Awaitable {
    HandlesRegistry &registry;
    int value = 0;

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        std::cout << "Suspended with value: " << value << "\n";
        registry.RegisterHandle(h, value);
    }

    void await_resume() {
    }
  };
  return Awaitable{registry, value};
}

static cobalt::promise<void> HandleValue(HandlesRegistry &registry,
                                         int value) {
  std::cout << "Before waiting for value: " << value << '\n';
  co_await OnValue(registry, value);
  std::cout << "After waiting for value: " << value << '\n';
}

static auto HandleIncomingMessages(HandlesRegistry &registry) {
  return cobalt::race(HandleValue(registry, 5), 
                      HandleValue(registry, 1),
                      HandleValue(registry, 1),
                      HandleValue(registry, 3));
}

static cobalt::task<void> DistributeIncomingMessages(
  HandlesRegistry &registry, cobalt::generator<int> &socket) {
  for (auto value = co_await socket; value != -1; value = co_await socket) {
    if (auto search = registry.handles.find(value); 
             search != registry.handles.end()) {
      for (auto handle: search->second) {
        std::cout << "Found handle waiting for value: " << value
                  << " resuming it\n";
        handle.resume();
      }
    }
  }
}

static cobalt::task<void>  DelayMs(size_t ms) {
  asio::steady_timer timer{co_await cobalt::this_coro::executor, 
                           std::chrono::milliseconds(ms)}; 
  co_await timer.async_wait(cobalt::use_op);
}

static cobalt::detached SpeakWithDelay() {
  std::cout << "SpeakWithDelay started\n";
  co_await DelayMs(3000);
  std::cout << "SpeakWithDelay after 3 seconds!\n";
}

cobalt::main co_main(int , char **) {
  SubscribableSocket socket{FromSocket(), {}};
  std::cout << "Before distributing messages\n";
  SpeakWithDelay();
  co_await HttpResponce();
  co_await cobalt::race(
      DistributeIncomingMessages(socket.registry, socket.socket),
      HandleIncomingMessages(socket.registry));
  std::cout << "After distributing messages\n";
  co_return 0;
}

// cobalt::promise<something> ImagineHandling() {

  
  //co_await async_write(socket, boost::asio::buffer(data, n));
  // co_await async_write(socket, SomeCommand);
  // auto event_data = co_await async_read(socket, "event_name");
  //  or
  //  auto event_data = co_await ExpectEvent<EventType/OrEventName/OrEventEnum>(socket, timeout);
// }

// Example - teleport between maps
// [[nodiscard]] cobalt::promise<bool?error?>  TransportTo(MapName map, SpawnId spawn_id) nothrow {
// 
// }

// Usage 
// cobalt::promise<void> TestTransportTo() {
  // auto player = co_await ConnectPlayer(...);
  // if (!co_await player.TransportTo("main", 1)) {
  // handle error
  // }
  // co_await MoveTo()....
// }

// Idea 2.
// Socket is actually a generator of Events, on which i can iterate untill it is runout of events, meaning connection is
// lost.
// Probably i need to co_await on values then.


