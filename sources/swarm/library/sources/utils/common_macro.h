// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include <cassert>

namespace swarm {

#ifndef F_INLINE
#if defined(_MSC_VER)
#define F_INLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define F_INLINE inline __attribute__((__always_inline__))
#else
#define F_INLINE inline
#endif
#endif

#define CONCAT_INNER(a, b) a##b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define MUST_BE(x)                                        \
  [[maybe_unused]] const auto CONCAT(res_, __LINE__) = x; \
  assert(CONCAT(res_, __LINE__) == 1);

struct CheckRes {
  F_INLINE explicit CheckRes(int expected_res) noexcept
      : expected_res_{expected_res} {}
  ~CheckRes() { assert(res_ == expected_res_); }

  F_INLINE auto operator=(int res) noexcept -> CheckRes& {
    res_ = res;
    return *this;
  }

  int res_ = 0;
  const int expected_res_ = 0;
};

#define MUST1 CheckRes{1} =

}  // namespace swarm

#endif  // COMMON_MACRO_H
