// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <memory.h>

#include <memory_helpers.h>

#include <doctest/doctest.h>

static void DeleterFunctionMock(gsl::owner<const int*> resource) noexcept {
  delete resource;
}

namespace glassy::test {

SCENARIO("DeleterFromFunction can be used with std::unique_ptr without leaks") {
  std::unique_ptr<int, DeleterFromFunction<DeleterFunctionMock>> ptr{
      new int{32}};

  CHECK_EQ(*ptr, 32);
}

}  // namespace glassy::test
