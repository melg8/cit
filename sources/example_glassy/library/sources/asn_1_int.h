// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef ASN_1_INT_H
#define ASN_1_INT_H

#include <memory>
#include <utility>

#include <compare>

#include <asn_1_int_errc.h>

#include <openssl/asn1.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

namespace glassy {

using Long = long;  // NOLINT
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

using Asn1IntegerOwnerPtr = gsl::owner<ASN1_INTEGER*>;
using Asn1IntegerNotNull = gsl::not_null<ASN1_INTEGER*>;
using Asn1IntegerConstNotNull = gsl::not_null<const ASN1_INTEGER*>;

struct Asn1IntDeleter {
  void operator()(Asn1IntegerOwnerPtr number) const noexcept;
};

inline void Asn1IntDeleter::operator()(
    Asn1IntegerOwnerPtr number) const noexcept {
  ASN1_INTEGER_free(number);
}

using Asn1IntHolder = std::unique_ptr<ASN1_INTEGER, Asn1IntDeleter>;
using Asn1IntOwner = gsl::not_null<Asn1IntHolder>;

template <class T>
using RawTypeOf = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;

template <class T, typename Target>
concept is_not_null_of_concrete_raw_pointer =
    std::same_as<RawTypeOf<T>, gsl::not_null<Target>>;

template <class T, typename Target>
concept is_not_null_owner_of_concrete_raw_pointer = requires(T provider) {
  { provider.get() } -> std::convertible_to<Target>;
};  // NOLINT

template <class T>
using ElementPointer = typename RawTypeOf<T>::element_type*;

template <class T>
using ElementType = typename RawTypeOf<T>::element_type;

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

static_assert(is_not_null<Asn1IntOwner>);
static_assert(!is_not_null<Asn1IntHolder>);
static_assert(is_not_null<Asn1IntegerNotNull>);
static_assert(is_not_null<Asn1IntegerConstNotNull>);

template <class T>
concept is_not_null_of_raw_pointer =
    is_not_null<T> && is_not_null_of_concrete_raw_pointer<T, ElementPointer<T>>;

static_assert(is_not_null_of_raw_pointer<Asn1IntegerNotNull>);

template <class T, typename Target>
concept not_null_provider_of = is_not_null<T> &&
    (is_not_null_of_concrete_raw_pointer<T, Target> ||
     is_not_null_owner_of_concrete_raw_pointer<T, Target>);

static_assert(not_null_provider_of<Asn1IntOwner, const ASN1_INTEGER*>);
static_assert(!not_null_provider_of<Asn1IntHolder, const ASN1_INTEGER*>);

class Asn1Int {
 public:
  static Result<Asn1IntOwner> New(Long value = 0) noexcept;

  static Result<Asn1IntOwner> New(
      not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept;

  static Result<Asn1IntOwner> Own(Asn1IntegerOwnerPtr ptr) noexcept;

 private:
  static Result<Asn1IntOwner> NewUninitialized() noexcept;
};

Result<Long> ToLong(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept;

decltype(auto) GetPtr(is_not_null_of_raw_pointer auto&& lhs) { return lhs; }

decltype(auto) GetPtr(auto&& lhs) { return lhs.get(); }

std::strong_ordering Compare(
    not_null_provider_of<const ASN1_INTEGER*> auto&& lhs,
    not_null_provider_of<const ASN1_INTEGER*> auto&& rhs) noexcept {
  return ASN1_INTEGER_cmp(GetPtr(lhs), GetPtr(rhs)) <=> 0;
}

inline glassy::Result<glassy::Asn1IntOwner>
Asn1Int::NewUninitialized() noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_new()};
  if (!ptr) {
    return Asn1IntErrc::kAllocationFailure;
  }
  return glassy::Result<Asn1IntOwner>{std::move(ptr)};
}

inline glassy::Result<glassy::Asn1IntOwner> glassy::Asn1Int::New(
    Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1Int::NewUninitialized());
  if (ASN1_INTEGER_set(result.get(), value) == 0) {
    return Asn1IntErrc::kAllocationFailure;
  }
  return {std::move(result)};
}

inline glassy::Result<glassy::Asn1IntOwner> Asn1Int::New(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_dup(GetPtr(view))};
  if (!ptr) {
    return Asn1IntErrc::kCopyFailure;
  }
  return {std::move(ptr)};
}

inline glassy::Result<glassy::Asn1IntOwner> Asn1Int::Own(
    Asn1IntegerOwnerPtr ptr) noexcept {
  if (!ptr) {
    return Asn1IntErrc::kNullPointerFailure;
  }
  return glassy::Result<glassy::Asn1IntOwner>{Asn1IntHolder{ptr}};
}

inline Result<Long> ToLong(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const auto result = ASN1_INTEGER_get(GetPtr(view));
  if (result == -1) {
    return Asn1IntErrc::kConversionFailure;
  }
  return result;
}

}  // namespace glassy

#endif  // ASN_1_INT_H
