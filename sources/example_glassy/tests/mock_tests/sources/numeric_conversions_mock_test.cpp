// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <openssl/asn1.h>
#include <openssl/bn.h>

extern "C" {

static BIGNUM* AlwaysFailAsn1IntegerToBn(const ASN1_INTEGER*,
                                         BIGNUM*) noexcept {
  return nullptr;
}
static ASN1_INTEGER* AlwaysFailBnToAsn1Integer(const BIGNUM*, ASN1_INTEGER*) {
  return nullptr;
}
}

#define ASN1_INTEGER_to_BN AlwaysFailAsn1IntegerToBn
#define BN_to_ASN1_INTEGER AlwaysFailBnToAsn1Integer

#include <numeric_conversions.h>

namespace glassy::test {

SCENARIO("numeric conversions failure") {
  SECTION("failing to create BigNum from Asn1Int") {
    const auto value = Asn1IntegerFrom(32);
    const auto result = convert::FromAsn1Int(value.value());
    CHECK(!result.has_value());
  }
  SECTION("failing to create Asn1Integer from BigNum") {
    const auto value = BigNum::New(32);
    const auto result = convert::FromBigNum(value.value());
    CHECK(!result.has_value());
  }
  SECTION("failing to update Asn1Integer from BigNum") {
    const auto value = BigNum::New(32);
    auto for_update = Asn1IntegerFrom(32);
    const auto result = convert::FromBigNum(value.value(), for_update.value());
    CHECK(!result.has_value());
  }
}

}  // namespace glassy::test
