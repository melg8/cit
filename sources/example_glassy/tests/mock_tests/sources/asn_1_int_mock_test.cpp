// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <openssl/asn1.h>

extern "C" {
static bool should_fail_alloc = true;
static bool should_fail_to_set_value = true;

static ASN1_INTEGER* MockAsn1IntegerNew() noexcept {
  return should_fail_alloc ? nullptr : ASN1_INTEGER_new();
}

// NOLINTNEXTLINE
static int MockAsn1IntegerSet(ASN1_INTEGER* a, long v) noexcept {
  return should_fail_to_set_value ? 0 : ASN1_INTEGER_set(a, v);
}

// NOLINTNEXTLINE
static long AlwaysFailAsn1IntegerGet(const ASN1_INTEGER*) noexcept {
  return -1;
}

static ASN1_INTEGER* MockAsn1IntegerDup(const ASN1_INTEGER*) noexcept {
  return nullptr;
}
}

#define ASN1_INTEGER_new MockAsn1IntegerNew
#define ASN1_INTEGER_set MockAsn1IntegerSet
#define ASN1_INTEGER_get AlwaysFailAsn1IntegerGet
#define ASN1_INTEGER_dup MockAsn1IntegerDup

#include <asn_1_integer.h>

namespace glassy::test {

SCENARIO("Asn1Integer failures") {
  SECTION("failing to create Asn1Integer due to allocation failure") {
    should_fail_alloc = true;
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
    should_fail_alloc = false;
    should_fail_to_set_value = true;
    {
      const auto result = Asn1IntegerFrom(0);
      CHECK(!result.has_value());
    }
  }
  SECTION("failing to convert Asn1Integer to long") {
    should_fail_alloc = false;
    should_fail_to_set_value = false;
    {
      const auto value = Asn1IntegerFrom(0).value();
      const auto result = Asn1IntegerGet(value);
      CHECK(!result.has_value());
    }
  }
  SECTION("failing to copy Asn1Integer value") {
    should_fail_alloc = false;
    should_fail_to_set_value = false;
    {
      const auto value = Asn1IntegerFrom(32).value();
      const auto result = Asn1IntegerDup(value);
      CHECK(!result.has_value());
    }
  }
}

}  // namespace glassy::test
