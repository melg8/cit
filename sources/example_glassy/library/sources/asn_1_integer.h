// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef ASN_1_INTEGER_H
#define ASN_1_INTEGER_H

#include <memory>
#include <utility>

#include <compare>

#include <asn_1_integer_errc.h>
#include <common_macro.h>
#include <memory_helpers.h>
#include <not_null_concepts.h>

#include <openssl/asn1.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

namespace glassy {

// TODO(melg): add strong types library to deal with built-in types.
using Long = long;  // NOLINT
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

using Asn1IntegerOwnerPtr = gsl::owner<ASN1_INTEGER*>;
using Asn1IntegerNotNull = gsl::not_null<ASN1_INTEGER*>;
using Asn1IntegerConstNotNull = gsl::not_null<const ASN1_INTEGER*>;
using Asn1IntegerMaybeNull =
    std::unique_ptr<ASN1_INTEGER, DeleterFromFunction<ASN1_INTEGER_free>>;

using Asn1Integer = gsl::not_null<Asn1IntegerMaybeNull>;

constexpr FORCEINLINE std::strong_ordering Asn1IntegerCmp(
    not_null_provider_of<const ASN1_INTEGER*> auto&& lhs,
    not_null_provider_of<const ASN1_INTEGER*> auto&& rhs) noexcept {
  return ASN1_INTEGER_cmp(GetPtr(lhs), GetPtr(rhs)) <=> 0;
}

template <is_not_null T>
struct ComparisonValueHolder {
  T ptr;  // NOLINT

  template <typename F>
  inline constexpr auto operator<=>(
      const ComparisonValueHolder<F>& rhs) const noexcept {
    return Asn1IntegerCmp(this->ptr, rhs.ptr);
  }

  template <typename F>
  inline constexpr bool operator==(
      const ComparisonValueHolder<F>& rhs) const noexcept {
    return std::is_eq(*this <=> rhs);
  }
};

template <is_not_null T>
static inline decltype(auto) ValueOf(T&& provider) noexcept {
  return ComparisonValueHolder<T>{std::forward<T>(provider)};
}

FORCEINLINE Result<Asn1Integer> Asn1IntegerNew() noexcept {
  Asn1IntegerMaybeNull ptr{ASN1_INTEGER_new()};
  return ptr ? Result<Asn1Integer>{std::move(ptr)}
             : Asn1IntegerErrc::kAllocationFailure;
}

static inline Result<Asn1Integer> Asn1IntegerDup(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  Asn1IntegerMaybeNull ptr{ASN1_INTEGER_dup(GetPtr(view))};
  return ptr ? Result<Asn1Integer>{std::move(ptr)}
             : Asn1IntegerErrc::kCopyFailure;
}

// TODO(melg): mark as deprecated, implement ASN1_INTEGER_get_int64 to be used
// instead.
static inline Result<Long> Asn1IntegerGet(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const auto result = ASN1_INTEGER_get(GetPtr(view));
  return result != -1 ? Result<Long>{result}
                      : Asn1IntegerErrc::kConversionFailure;
}

FORCEINLINE Result<void> Asn1IntegerSet(
    not_null_provider_of<ASN1_INTEGER*> auto&& view, Long value) noexcept {
  return ASN1_INTEGER_set(GetPtr(view), value) != 0
             ? Result<void>{outcome::success()}
             : Asn1IntegerErrc::kAllocationFailure;
}

FORCEINLINE Result<Asn1Integer> Asn1IntegerFrom(Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1IntegerNew());
  OUTCOME_TRY(Asn1IntegerSet(result, value));
  return Result<Asn1Integer>{std::move(result)};
}

static inline Result<Asn1Integer> Own(Asn1IntegerOwnerPtr ptr) noexcept {
  return ptr ? Result<Asn1Integer>{Asn1IntegerMaybeNull{ptr}}
             : Asn1IntegerErrc::kNullPointerFailure;
}

}  // namespace glassy

#endif  // ASN_1_INTEGER_H
