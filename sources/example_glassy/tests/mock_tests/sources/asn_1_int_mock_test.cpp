// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <openssl/asn1.h>

extern "C" {
namespace {
extern bool int_should_fail_alloc;
bool int_should_fail_alloc = true;

extern bool int_should_fail_to_set_value;
bool int_should_fail_to_set_value = true;

ASN1_INTEGER* MockAsn1IntegerNew() noexcept {
  return int_should_fail_alloc ? nullptr : ASN1_INTEGER_new();
}

// NOLINTNEXTLINE
int MockAsn1IntegerSet(ASN1_INTEGER* a, long v) noexcept {
  return int_should_fail_to_set_value ? 0 : ASN1_INTEGER_set(a, v);
}

// NOLINTNEXTLINE
long AlwaysFailAsn1IntegerGet(const ASN1_INTEGER*) noexcept {
  return -1;
}

ASN1_INTEGER* MockAsn1IntegerDup(const ASN1_INTEGER*) noexcept {
  return nullptr;
}

}  // namespace
}

#define ASN1_INTEGER_new MockAsn1IntegerNew
#define ASN1_INTEGER_set MockAsn1IntegerSet
#define ASN1_INTEGER_get AlwaysFailAsn1IntegerGet
#define ASN1_INTEGER_dup MockAsn1IntegerDup

#include <asn_1_integer.h>

namespace glassy::test {

SCENARIO("Asn1Integer failures") {
  SECTION("failing to create Asn1Integer due to allocation failure") {
    int_should_fail_alloc = true;
    {
      const auto result = Asn1IntegerFrom(0);
      CHECK(!result.has_value());
    }
    {
      const auto result = Asn1IntegerFrom(32);
      CHECK(!result.has_value());
    }
  }

  SECTION("failing to create Asn1Integer due to set value failure") {
    int_should_fail_alloc = false;
    int_should_fail_to_set_value = true;
    {
      const auto result = Asn1IntegerFrom(0);
      CHECK(!result.has_value());
    }
  }
  SECTION("failing to convert Asn1Integer to long") {
    int_should_fail_alloc = false;
    int_should_fail_to_set_value = false;
    {
      const auto value = Asn1IntegerFrom(0).value();
      const auto result = Asn1IntegerGet(value);
      CHECK(!result.has_value());
    }
  }
  SECTION("failing to copy Asn1Integer value") {
    int_should_fail_alloc = false;
    int_should_fail_to_set_value = false;
    {
      const auto value = Asn1IntegerFrom(32).value();
      const auto result = Asn1IntegerDup(value);
      CHECK(!result.has_value());
    }
  }
}

}  // namespace glassy::test
