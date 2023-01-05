// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_int.h>
#include <big_num.h>
#include <doctest/doctest.h>
#include <numeric_conversions.h>
#include <outcome.hpp>

#define O_TRY OUTCOME_TRY

namespace glassy::test {

static Result<Asn1IntOwner> ProvideAsn1Value() noexcept {
  O_TRY(auto asn_1_int, New(32));
  O_TRY(auto bignum, convert::FromAsn1Int(asn_1_int));
  O_TRY(bignum += BigNum::New(1));
  O_TRY(convert::FromBigNum(bignum, asn_1_int));
  return {std::move(asn_1_int)};
}

SCENARIO("Asn1Int and Bignum usability test") {
  const auto asn_1_int = ProvideAsn1Value();
  CHECK(asn_1_int.has_value());
  CHECK(std::is_eq(Compare(asn_1_int.value(), New(33).value())));
}

static ASN1_INTEGER* ProvideAsn1Pointer() noexcept {
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

SCENARIO("openssl usability test") {
  ASN1_INTEGER* result = ProvideAsn1Pointer();
  CHECK_NE(result, nullptr);

  ASN1_INTEGER* expected = ASN1_INTEGER_new();

  // Mistake 1: used result instead of expected.
  CHECK_NE(ASN1_INTEGER_set(expected, 33), 0);

  CHECK_EQ(ASN1_INTEGER_cmp(result, expected), 0);

  ASN1_INTEGER_free(result);
  ASN1_INTEGER_free(expected);
}

struct Asn1IntegerDeleter {
  void operator()(ASN1_INTEGER* number) const noexcept {
    ASN1_INTEGER_free(number);
  }
};

using Asn1Integer = std::unique_ptr<ASN1_INTEGER, Asn1IntegerDeleter>;

struct BignumDeleter {
  void operator()(BIGNUM* number) const noexcept { BN_free(number); }
};

using Asn1BigNum = std::unique_ptr<BIGNUM, BignumDeleter>;

static Asn1Integer ProvideAsn1SmartPointer() noexcept {
  Asn1Integer result{ASN1_INTEGER_new()};
  if (result == nullptr) {
    return nullptr;
  }
  if (ASN1_INTEGER_set(result.get(), 32) == 0) {
    return nullptr;
  }
  Asn1BigNum bignum{ASN1_INTEGER_to_BN(result.get(), nullptr)};
  if (bignum == nullptr) {
    return nullptr;
  }
  if (BN_add(bignum.get(), bignum.get(), BN_value_one()) == 0) {
    return nullptr;
  }
  if (BN_to_ASN1_INTEGER(bignum.get(), result.get()) == nullptr) {
    return nullptr;
  }
  return result;
}

SCENARIO("openssl usability test 1") {
  Asn1Integer result = ProvideAsn1SmartPointer();
  CHECK_NE(result, nullptr);

  Asn1Integer expected{ASN1_INTEGER_new()};

  CHECK_NE(ASN1_INTEGER_set(expected.get(), 33), 0);
  CHECK_EQ(ASN1_INTEGER_cmp(result.get(), expected.get()), 0);
}

}  // namespace glassy::test
