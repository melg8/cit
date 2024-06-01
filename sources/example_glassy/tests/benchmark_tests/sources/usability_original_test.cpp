// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <openssl/asn1.h>

namespace glassy::test {

inline auto ProvideAsn1Pointer() noexcept -> ASN1_INTEGER* {
  ASN1_INTEGER* result = ASN1_INTEGER_new();
  if (result == nullptr) {
    return nullptr;
  }
  if (ASN1_INTEGER_set(result, 32) == 0) {
    ASN1_INTEGER_free(result);
    return nullptr;
  }
  BIGNUM* bignum = ASN1_INTEGER_to_BN(result, nullptr);
  if (bignum == nullptr) {
    ASN1_INTEGER_free(result);
    return nullptr;
  }
  if (BN_add(bignum, bignum, BN_value_one()) == 0) {
    BN_free(bignum);
    ASN1_INTEGER_free(result);
    return nullptr;
  }
  if (BN_to_ASN1_INTEGER(bignum, result) == nullptr) {
    BN_free(bignum);
    ASN1_INTEGER_free(result);
    return nullptr;
  }
  BN_free(bignum);  // Mistake 2: forgot this line.
  return result;
}

inline auto ProvideWithValue() noexcept -> ASN1_INTEGER* {
  ASN1_INTEGER* result = ASN1_INTEGER_new();
  if (result == nullptr) {
    return nullptr;
  }
  if (ASN1_INTEGER_set(result, 31) == 0) {
    ASN1_INTEGER_free(result);
    return nullptr;
  }
  return result;
}

SCENARIO("openssl usability") {
  BENCHMARK("creation with value") {
    ASN1_INTEGER* result = ProvideWithValue();
    CHECK(result != nullptr);

    ASN1_INTEGER* expected = ASN1_INTEGER_new();

    // Initially had mistake 1: used result instead of expected.
    // Potential leak if check fails.
    CHECK(ASN1_INTEGER_set(expected, 31) != 0);
    CHECK(ASN1_INTEGER_cmp(result, expected) == 0);

    ASN1_INTEGER_free(result);
    ASN1_INTEGER_free(expected);
  };
}

SCENARIO("openssl isolated") {
  BENCHMARK("compare raw pointers") {
    ASN1_INTEGER* result = ProvideAsn1Pointer();
    CHECK(result != nullptr);

    ASN1_INTEGER* expected = ASN1_INTEGER_new();

    // Initially had mistake 1: used result instead of expected.
    // Potential leak if check fails.
    CHECK(ASN1_INTEGER_set(expected, 33) != 0);
    CHECK(ASN1_INTEGER_cmp(result, expected) == 0);

    ASN1_INTEGER_free(result);
    ASN1_INTEGER_free(expected);
  };
}

}  // namespace glassy::test
