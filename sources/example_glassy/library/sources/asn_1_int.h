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

inline Result<Asn1IntOwner> Asn1Int::NewUninitialized() noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_new()};
  return ptr ? Result<Asn1IntOwner>{std::move(ptr)}
             : Asn1IntErrc::kAllocationFailure;
}

inline Result<Asn1IntOwner> Asn1Int::New(Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1Int::NewUninitialized());
  return ASN1_INTEGER_set(result.get(), value) != 0
             ? Result<Asn1IntOwner>{std::move(result)}
             : Asn1IntErrc::kAllocationFailure;
}

inline Result<Asn1IntOwner> Asn1Int::New(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_dup(GetPtr(view))};
  return ptr ? Result<Asn1IntOwner>{std::move(ptr)} : Asn1IntErrc::kCopyFailure;
}

inline Result<Asn1IntOwner> Asn1Int::Own(Asn1IntegerOwnerPtr ptr) noexcept {
  return ptr ? Result<Asn1IntOwner>{Asn1IntHolder{ptr}}
             : Asn1IntErrc::kNullPointerFailure;
}

inline Result<Long> ToLong(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const auto result = ASN1_INTEGER_get(GetPtr(view));
  return result != -1 ? Result<Long>{result} : Asn1IntErrc::kConversionFailure;
}

}  // namespace glassy

#endif  // ASN_1_INT_H
