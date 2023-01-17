// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#ifndef FORCEINLINE
#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define FORCEINLINE inline __attribute__((__always_inline__))
#else
#define FORCEINLINE inline
#endif
#endif

#endif  // COMMON_MACRO_H
