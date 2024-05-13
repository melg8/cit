// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef PRETTY_JSON_FROM_ANY_STRUCT_H
#define PRETTY_JSON_FROM_ANY_STRUCT_H

#include <glaze/glaze.hpp>

#include <string>

namespace coal {

template <typename T>
[[nodiscard]] std::string JsonFrom(const T& any_struct) {
  std::string result{};
  glz::write<glz::opts{.prettify = true}>(any_struct, result);
  return result;
}

//
}  // namespace coal

#endif  // PRETTY_JSON_FROM_ANY_STRUCT_H