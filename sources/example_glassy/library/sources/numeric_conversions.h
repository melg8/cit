// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef NUMERIC_COVERSIONS_H
#define NUMERIC_COVERSIONS_H

#include <asn_1_integer.h>
#include <big_num.h>

namespace glassy::convert {

namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

static Result<BigNum> FromAsn1Int(
    not_null_provider_of<const ASN1_INTEGER*> auto&& value) noexcept;
static Result<Asn1Integer> FromBigNum(const BigNum& value) noexcept;
static Result<void> FromBigNum(
    const BigNum& value,
    not_null_provider_of<const ASN1_INTEGER*> auto&& target) noexcept;

inline Result<BigNum> FromAsn1Int(
    not_null_provider_of<const ASN1_INTEGER*> auto&& value) noexcept {
  auto result = BigNum::Own(ASN1_INTEGER_to_BN(GetPtr(value), nullptr));
  if (result.has_error()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

inline Result<Asn1Integer> FromBigNum(const BigNum& value) noexcept {
  auto result = Own(BN_to_ASN1_INTEGER(value.Ptr(), nullptr));
  if (result.has_error()) {
    return Asn1IntegerErrc::kConversionFailure;
  }
  return result;
}

inline Result<void> FromBigNum(
    const BigNum& value,
    not_null_provider_of<const ASN1_INTEGER*> auto&& target) noexcept {
  if (BN_to_ASN1_INTEGER(value.Ptr(), GetPtr(target)) == nullptr) {
    return Asn1IntegerErrc::kConversionFailure;
  }
  return outcome::success();
}

}  // namespace glassy::convert

#endif  // NUMERIC_COVERSIONS_H
