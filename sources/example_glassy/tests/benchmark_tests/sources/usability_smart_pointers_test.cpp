// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <memory>

#include <openssl/asn1.h>

#include <testing_framework.h>

namespace glassy::test {

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

SCENARIO("openssl usability with smart pointers") {
  BENCHMARK("compare smart pointers") {
    Asn1Integer result = ProvideAsn1SmartPointer();
    CHECK(result != nullptr);

    Asn1Integer expected{ASN1_INTEGER_new()};

    CHECK(ASN1_INTEGER_set(expected.get(), 33) != 0);
    CHECK(ASN1_INTEGER_cmp(result.get(), expected.get()) == 0);
  };
}

}  // namespace glassy::test
