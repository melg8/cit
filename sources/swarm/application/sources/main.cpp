// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

auto co_main(int, char**) -> boost::cobalt::main {
  std::cout << "The swarmy server has started.\n";
  std::cout.flush();
  co_return 0;
}

