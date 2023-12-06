// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <memory.h>

#include <memory_helpers.h>

#include <testing_framework.h>

namespace glassy::test {

void DeleterFunctionMock(gsl::owner<const int*> resource) noexcept;
void DeleterFunctionMock(gsl::owner<const int*> resource) noexcept {
  delete resource;
}

SCENARIO("DeleterFromFunction can be used with std::unique_ptr without leaks") {
  const std::unique_ptr<int, DeleterFromFunction<DeleterFunctionMock>> ptr{
      new int{32}};

  CHECK(*ptr == 32);
}

}  // namespace glassy::test
