// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

// Copyright (c) 2023 Matthijs Möhlmann
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <boost/cobalt/race.hpp>
#include <boost/cobalt/this_thread.hpp>
#include <boost/cobalt/this_coro.hpp>
#include <boost/cobalt/main.hpp>

#include <map>
#include <vector>
#include <coroutine>
#include <stdexcept>
#include <iostream>

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
};

static auto OnValue(HandlesRegistry &registry, int value) {
  struct awaitable {
    HandlesRegistry &registry;
    int value = 0;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        std::cout << "Suspended with value: " << value << "\n";
        registry.RegisterHandle(h, value);
    }
    void await_resume() {}
  };
  return awaitable{registry, value};
}

static cobalt::promise<void> HandleValue(HandlesRegistry &registry,
                                                int value) {
  std::cout << "Before waiting for value: " << value << '\n';
  co_await OnValue(registry, value);
  std::cout << "After waiting for value: " << value << '\n';
}

static auto HandleIncomingMessages(HandlesRegistry &registry) {
  return cobalt::race(HandleValue(registry, 5), HandleValue(registry, 1),
                      HandleValue(registry, 1), HandleValue(registry, 3));
}

static cobalt::task<void> DistributeIncomingMessages(HandlesRegistry &registry,
                                              cobalt::generator<int> &socket) {
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

cobalt::main co_main(int , char **) {
  auto socket = FromSocket();
  HandlesRegistry registry;
  std::cout << "Before distributing messages\n";
  co_await cobalt::race(DistributeIncomingMessages(registry, socket), 
                        HandleIncomingMessages(registry));
  std::cout << "After distributing messages\n";
  co_return 0;
}

// extern cobalt::generator<int> test();
// cobalt::generator<int> test() {
//   printf("test-1\n");
//   co_yield 1;
//   printf("test-2\n");
//   co_yield 2;
//   printf("test-3\n");
//   co_return 3;
// }

// static cobalt::generator<int> test(int max) {
//   int i = 0;
//   while (i < max)
//     co_yield i++;

//   co_return i;
// }

// cobalt::main co_main(int , char ** ) {
//     std::cout << "Main-1\n";
//     auto tt = test(10);
//     std::cout << "Main-2\n";
//     std::cout << "Main-3 " << co_await tt << "\n";
//     std::cout << "Main-4 " << co_await tt << "\n";
//     std::cout << "Main-5 " << co_await tt << "\n";
//     co_return 0;
// }

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


