// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <limits>

#include <doctest/doctest.h>

#include <openssl/bn.h>

extern "C" {
static bool should_fail_alloc = true;
static bool should_fail_one_time_set_word = false;
static BIGNUM* MockBnNew() noexcept {
  return should_fail_alloc ? nullptr : BN_new();
}

static int BnSetWord(BIGNUM* a, BN_ULONG w) noexcept {
  if (should_fail_one_time_set_word) {
    should_fail_one_time_set_word = false;
    return 0;
  }
  return BN_set_word(a, w);
}

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

static int AlwaysFailBnToBin(const BIGNUM*, unsigned char*) { return -1; }
}

#define BN_new MockBnNew
#define BN_get_word AlwaysFailBnGetWord
#define BN_bn2dec AlwaysFailBnTo
#define BN_dec2bn AlwaysFailToBn
#define BN_bn2hex AlwaysFailBnTo
#define BN_hex2bn AlwaysFailToBn
#define BN_bin2bn AlwaysFailBinToBn
#define BN_add AlwaysFailAdd
#define BN_bn2bin AlwaysFailBnToBin
#define BN_set_word BnSetWord

#include <big_num.h>

namespace glassy {
namespace test {

SCENARIO("BigNum failures") {
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
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("any BnUlong value") {
    should_fail_alloc = false;
    const BnUlong value = 1;

    WHEN("failed to create Bignum from it because of expansion error") {
      should_fail_one_time_set_word = true;
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("default created BigNum") {
    should_fail_alloc = false;
    const auto result = BigNum::New();

    WHEN("failed to convert to BnUlong because of internal error") {
      const auto converted = result.value().ToBnUlong();

      THEN("converted doesn't have value") {
        CHECK_FALSE(converted.has_value());
      }
    }

    WHEN("failed to convert to dec") {
      const auto converted = result.value().ToDec();

      THEN("result doesn't have value") { CHECK_FALSE(converted.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    should_fail_alloc = false;
    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::New(Dec{"4"});

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    should_fail_alloc = false;
    const auto value = BigNum::New(15);

    WHEN("converting it to dec") {
      const auto result = value.value().ToHex();

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    should_fail_alloc = false;
    const auto value = BigNum::New(15);

    WHEN("converting it to bin") {
      const auto result = value.value().ToBin();

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    should_fail_alloc = false;
    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::New(Hex{"0F"});

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  GIVEN("bin value of BigNum") {
    should_fail_alloc = false;
    SslData value{10};

    WHEN("failed to create BigNum from it") {
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK_FALSE(result.has_value()); }
    }
  }

  []() -> Result<void> {
    SUBCASE("add two BigNum failing") {
      should_fail_alloc = false;
      OUTCOME_TRY(const auto first, BigNum::New(2));
      OUTCOME_TRY(const auto second, BigNum::New(3));
      const auto result = BigNum::Add(first, second);
      CHECK_FALSE(result.has_value());
    }

    SUBCASE("BigNum += BigNum failing") {
      should_fail_alloc = false;

      OUTCOME_TRY(auto value, BigNum::New(2));
      OUTCOME_TRY(const auto second_value, BigNum::New(3));
      CHECK_FALSE((value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK_EQ(value, result);
    }

    SUBCASE("BigNum += Result<BigNum> failing") {
      should_fail_alloc = false;

      OUTCOME_TRY(auto value, BigNum::New(2));
      CHECK_FALSE((value += BigNum::New(3)).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK_EQ(value, result);
    }

    SUBCASE("Result<BigNum> += BigNum") {
      should_fail_alloc = false;

      auto value = BigNum::New(2);
      OUTCOME_TRY(const auto second_value, BigNum::New(3));
      CHECK_FALSE((value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK_EQ(value.value(), result);
    }

    SUBCASE("Result<BigNum> += Result<BigNum>") {
      should_fail_alloc = false;

      auto value = BigNum::New(2);
      CHECK_FALSE((value += BigNum::New(3)).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK_EQ(value.value(), result);
    }

    SUBCASE("!Result<BigNum> += Result<BigNum>") {
      should_fail_alloc = false;

      Result<BigNum> value = BigNumErrc::kAllocationFailure;
      CHECK_FALSE((value += BigNum::New(3)).has_value());
      CHECK_FALSE(value.has_value());
    }

    SUBCASE("Result<BigNum> += !Result<BigNum>") {
      should_fail_alloc = false;

      auto value = BigNum::New(2);
      Result<BigNum> second_value = BigNumErrc::kAllocationFailure;
      CHECK_FALSE((value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK_EQ(value.value(), result);
    }
    return outcome_v2::success();
  }()
              .value();
}

}  // namespace test
}  // namespace glassy
