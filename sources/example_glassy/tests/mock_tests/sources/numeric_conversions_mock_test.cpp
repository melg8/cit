#include <doctest/doctest.h>

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

namespace glassy {
namespace test {

SCENARIO("numeric conversions failure") {
  SUBCASE("failing to create BigNum from Asn1Int") {
    const auto value = Asn1Int::New(32);
    const auto result = convert::FromAsn1Int(value.value());
    CHECK_FALSE(result.has_value());
  }
  SUBCASE("failing to create Asn1Int from BigNum ") {
    const auto value = BigNum::New(32);
    const auto result = convert::FromBigNum(value.value());
    CHECK_FALSE(result.has_value());
  }
}

}  // namespace test
}  // namespace glassy
