// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#ifndef F_INLINE
#if defined(_MSC_VER)
#define F_INLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define F_INLINE inline __attribute__((__always_inline__))
#else
#define F_INLINE inline
#endif
#endif

#endif  // COMMON_MACRO_H
