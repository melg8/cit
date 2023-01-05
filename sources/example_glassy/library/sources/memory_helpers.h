// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef MEMORY_HELPERS_H
#define MEMORY_HELPERS_H

#include <gsl/gsl-lite.hpp>

namespace glassy {

template <auto func>
struct DeleterFromFunction {
  template <typename T>
  constexpr void operator()(gsl::owner<T*> arg) const noexcept {
    func(arg);
  }
};

}  // namespace glassy

#endif  // MEMORY_HELPERS_H
