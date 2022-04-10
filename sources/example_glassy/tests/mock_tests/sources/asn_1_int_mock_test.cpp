#include <doctest/doctest.h>

#include <openssl/asn1.h>

extern "C" {
static bool should_fail_alloc = true;
static bool should_fail_to_set_value = true;

static ASN1_INTEGER* MockAsn1IntegerNew() noexcept {
  return should_fail_alloc ? nullptr : ASN1_INTEGER_new();
}

static int MockAsn1IntegerSet(ASN1_INTEGER* a, long v) {
  return should_fail_to_set_value ? 0 : ASN1_INTEGER_set(a, v);
}

static long AlwaysFailAsn1IntegerGet(const ASN1_INTEGER*) { return -1; }
}

#define ASN1_INTEGER_new MockAsn1IntegerNew
#define ASN1_INTEGER_set MockAsn1IntegerSet
#define ASN1_INTEGER_get AlwaysFailAsn1IntegerGet

#include <asn_1_int.h>

SCENARIO("Asn1Int failures") {
  using namespace glassy;
  SUBCASE("failing to create Asn1Int due to allocation failure") {
    should_fail_alloc = true;
    {
      const auto result = Asn1Int::New();
      CHECK_FALSE(result.has_value());
    }
    {
      const auto result = Asn1Int::New(32);
      CHECK_FALSE(result.has_value());
    }
  }

  SUBCASE("failing to create Asn1Int due set value failure") {
    should_fail_alloc = false;
    should_fail_to_set_value = true;
    {
      const auto result = Asn1Int::New();
      CHECK_FALSE(result.has_value());
    }
  }
  SUBCASE("failing to convert Asn1Int to long") {
    should_fail_alloc = false;
    should_fail_to_set_value = false;
    {
      const auto value = Asn1Int::New().value();
      const auto result = value.ToLong();
      CHECK_FALSE(result.has_value());
    }
  }
}
