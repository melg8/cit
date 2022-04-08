#include <doctest/doctest.h>

#include <openssl/bn.h>
#include <limits>

extern "C" {
static bool should_fail_alloc = true;
static BIGNUM* MockBnNew() { return should_fail_alloc ? nullptr : BN_new(); }

// For this function fail condition is represented by all 0xFF bytes
// in result. Search for BN_MASK2 in openssl source code for reference.
static auto BnGetWordFailConditionReturnValue() noexcept {
  return std::numeric_limits<decltype(BN_get_word(nullptr))>::max();
}

static BN_ULONG AlwaysFailBnGetWord(const BIGNUM*) noexcept {
  return BnGetWordFailConditionReturnValue();
}

static char* AlwaysFailBnTo(const BIGNUM*) noexcept { return nullptr; }

static int AlwaysFailToBn(BIGNUM**, const char*) noexcept { return 0; }

static BIGNUM* AlwaysFailBinToBn(const unsigned char*, int, BIGNUM*) {
  return nullptr;
}
static int AlwaysFailAdd(BIGNUM*, const BIGNUM*, const BIGNUM*) { return 0; }
}

#define BN_new MockBnNew
#define BN_get_word AlwaysFailBnGetWord
#define BN_bn2dec AlwaysFailBnTo
#define BN_dec2bn AlwaysFailToBn
#define BN_bn2hex AlwaysFailBnTo
#define BN_hex2bn AlwaysFailToBn
#define BN_bin2bn AlwaysFailBinToBn
#define BN_add AlwaysFailAdd

#include <bignum.h>

SCENARIO("bignum failures") {
  using namespace glassy;
  should_fail_alloc = true;
  GIVEN("creating new BigNum") {
    const auto result = BigNum::New();
    WHEN("failed to create BigNum because of internal error") {
      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("any BnUlong value") {
    const BnUlong value = 1;

    WHEN("failed to create Bignum from it because of internal error") {
      const auto result = BigNum::FromBnUlong(value);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("default created BigNum") {
    should_fail_alloc = false;
    const auto result = BigNum::New();

    WHEN("failed to convert to BnUlong because of internal error") {
      const auto converted = BigNum::ToBnUlong(result.value());

      THEN("converted doesn't have value") {
        CHECK_FALSE(converted.has_value());
      }
    }

    WHEN("failed to convert to dec") {
      const auto converted = BigNum::ToDec(result.value());

      THEN("result doesn't have value") { CHECK_FALSE(converted.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    should_fail_alloc = false;
    const auto* pointer = "4";

    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::FromDec(pointer);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    should_fail_alloc = false;
    const auto value = BigNum::FromBnUlong(15);

    WHEN("converting it to dec") {
      const auto result = BigNum::ToHex(value.value());

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    should_fail_alloc = false;
    const auto* pointer = "0F";

    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::FromHex(pointer);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("bin value of BigNum") {
    should_fail_alloc = false;
    SslData value{10};

    WHEN("failed to create BigNum from it") {
      const auto result = BigNum::FromBin(value);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  []() -> Result<void> {
    DOCTEST_SUBCASE("add two BigNumb failing") {
      should_fail_alloc = false;
      OUTCOME_TRY(const auto first, BigNum::FromBnUlong(2));
      OUTCOME_TRY(const auto second, BigNum::FromBnUlong(3));
      const auto result = BigNum::Add(first, second);
      CHECK_FALSE(result.has_value());
    }
    return outcome_v2::success();
  }()
              .value();
}
