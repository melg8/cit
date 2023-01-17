// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef NOT_NULL_CONCEPTS_H
#define NOT_NULL_CONCEPTS_H

#include <type_traits>

#include <concepts>

#include <gsl/gsl-lite.hpp>

namespace glassy {

template <class T>
using RawTypeOf = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;

template <class T>
using ElementPointer = typename RawTypeOf<T>::element_type*;

template <class T>
using ElementType = typename RawTypeOf<T>::element_type;

template <class T, typename Target>
concept is_not_null_of_concrete_raw_pointer =
    std::same_as<RawTypeOf<T>, gsl::not_null<Target>>;

template <class T, typename Target>
concept is_not_null_owner_of_concrete_raw_pointer = requires(T provider) {
  { provider.get() } -> std::convertible_to<Target>;
};  // NOLINT

template <typename T>
struct IsNotNull {
  static constexpr bool const value = false;
};

template <typename T>
struct IsNotNull<gsl::not_null<T>> {
  static constexpr bool const value = true;
};

template <class T>
concept is_not_null = IsNotNull<RawTypeOf<T>>::value;

template <class T>
concept is_not_null_of_raw_pointer =
    is_not_null<T> && is_not_null_of_concrete_raw_pointer<T, ElementPointer<T>>;

template <class T, typename Target>
concept not_null_provider_of = is_not_null<T> &&
    (is_not_null_of_concrete_raw_pointer<T, Target> ||
     is_not_null_owner_of_concrete_raw_pointer<T, Target>);

constexpr inline decltype(auto) GetPtr(
    is_not_null_of_raw_pointer auto&& lhs) noexcept {
  return lhs;
}

constexpr inline decltype(auto) GetPtr(is_not_null auto&& lhs) noexcept {
  return gsl::not_null{lhs.get()};
}

}  // namespace glassy

#endif  // NOT_NULL_CONCEPTS_H
