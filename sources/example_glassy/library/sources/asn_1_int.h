// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef ASN_1_INT_H
#define ASN_1_INT_H

#include <memory>
#include <utility>

#include <compare>

#include <asn_1_int_errc.h>
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

using Asn1IntHolder =
    std::unique_ptr<ASN1_INTEGER, DeleterFromFunction<ASN1_INTEGER_free>>;

using Asn1IntOwner = gsl::not_null<Asn1IntHolder>;

static inline std::strong_ordering Asn1IntegerCmp(
    not_null_provider_of<const ASN1_INTEGER*> auto&& lhs,
    not_null_provider_of<const ASN1_INTEGER*> auto&& rhs) noexcept {
  return ASN1_INTEGER_cmp(GetPtr(lhs), GetPtr(rhs)) <=> 0;
}

static inline Result<Asn1IntOwner> Asn1IntegerNew() noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_new()};
  return ptr ? Result<Asn1IntOwner>{std::move(ptr)}
             : Asn1IntErrc::kAllocationFailure;
}

static inline Result<Asn1IntOwner> Asn1IntegerDup(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_dup(GetPtr(view))};
  return ptr ? Result<Asn1IntOwner>{std::move(ptr)} : Asn1IntErrc::kCopyFailure;
}

// TODO(melg): mark as deprecated, implement ASN1_INTEGER_get_int64 to be used
// instead.
static inline Result<Long> Asn1IntegerGet(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const auto result = ASN1_INTEGER_get(GetPtr(view));
  return result != -1 ? Result<Long>{result} : Asn1IntErrc::kConversionFailure;
}

static inline Result<void> Asn1IntegerSet(
    not_null_provider_of<ASN1_INTEGER*> auto&& view, Long value) noexcept {
  return ASN1_INTEGER_set(GetPtr(view), value) != 0
             ? Result<void>{outcome::success()}
             : Asn1IntErrc::kAllocationFailure;
}

static inline Result<Asn1IntOwner> Asn1IntegerFrom(Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1IntegerNew());
  OUTCOME_TRY(Asn1IntegerSet(result, value));
  return Result<Asn1IntOwner>{std::move(result)};
}

static inline Result<Asn1IntOwner> Own(Asn1IntegerOwnerPtr ptr) noexcept {
  return ptr ? Result<Asn1IntOwner>{Asn1IntHolder{ptr}}
             : Asn1IntErrc::kNullPointerFailure;
}

}  // namespace glassy

#endif  // ASN_1_INT_H
