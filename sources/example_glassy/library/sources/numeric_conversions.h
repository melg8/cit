// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef NUMERIC_CONVERSIONS_H
#define NUMERIC_CONVERSIONS_H

#include <asn_1_integer.h>
#include <big_num.h>
#include <common_macro.h>

namespace glassy::convert {

namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

static auto FromAsn1Int(
    not_null_provider_of<const ASN1_INTEGER*> auto&& value) noexcept
    -> Result<BigNum>;
static auto FromBigNum(const BigNum& value) noexcept -> Result<Asn1Integer>;
static auto FromBigNum(
    const BigNum& value,
    not_null_provider_of<const ASN1_INTEGER*> auto&& target) noexcept
    -> Result<void>;

FORCEINLINE auto FromAsn1Int(
    not_null_provider_of<const ASN1_INTEGER*> auto&& value) noexcept
    -> Result<BigNum> {
  auto result = BigNum::Own(ASN1_INTEGER_to_BN(GetPtr(value), nullptr));
  if (result.has_error()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto FromBigNum(const BigNum& value) noexcept
    -> Result<Asn1Integer> {
  auto result = Own(BN_to_ASN1_INTEGER(value.Ptr(), nullptr));
  if (result.has_error()) {
    return Asn1IntegerErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto FromBigNum(
    const BigNum& value,
    not_null_provider_of<const ASN1_INTEGER*> auto&& target) noexcept
    -> Result<void> {
  if (BN_to_ASN1_INTEGER(value.Ptr(), GetPtr(target)) == nullptr) {
    return Asn1IntegerErrc::kConversionFailure;
  }
  return outcome::success();
}

}  // namespace glassy::convert

#endif  // NUMERIC_CONVERSIONS_H
