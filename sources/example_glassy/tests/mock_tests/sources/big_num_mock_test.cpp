// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <limits>

#include <testing_framework.h>

#include <openssl/bn.h>

extern "C" {

namespace {
extern bool big_num_should_fail_alloc;
bool big_num_should_fail_alloc = true;

extern bool big_num_should_fail_one_time_set_word;
bool big_num_should_fail_one_time_set_word = false;

BIGNUM* MockBnNew() noexcept {
  return big_num_should_fail_alloc ? nullptr : BN_new();
}

int BnSetWord(BIGNUM* big_num, BN_ULONG word) noexcept {
  if (big_num_should_fail_one_time_set_word) {
    big_num_should_fail_one_time_set_word = false;
    return 0;
  }
  return BN_set_word(big_num, word);
}

// For this function fail condition is represented by all 0xFF bytes
// in result. Search for BN_MASK2 in openssl source code for reference.
BN_ULONG BnGetWordFailConditionReturnValue() noexcept {
  return std::numeric_limits<decltype(BN_get_word(nullptr))>::max();
}

BN_ULONG AlwaysFailBnGetWord(const BIGNUM*) noexcept {
  return BnGetWordFailConditionReturnValue();
}

char* AlwaysFailBnTo(const BIGNUM*) noexcept { return nullptr; }

int AlwaysFailToBn(BIGNUM**, const char*) noexcept { return 0; }

BIGNUM* AlwaysFailBinToBn(const unsigned char*, int, BIGNUM*) {
  return nullptr;
}

int AlwaysFailAdd(BIGNUM*, const BIGNUM*, const BIGNUM*) { return 0; }

int AlwaysFailBnToBin(const BIGNUM*, unsigned char*) { return -1; }

}  // namespace
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

namespace glassy::test {

SCENARIO("BigNum failures") {
  big_num_should_fail_alloc = true;
  GIVEN("creating new BigNum") {
    const auto result = BigNum::New();
    WHEN("failed to create BigNum because of internal error") {
      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("any BnUlong value") {
    const BnUlong value = 1;

    WHEN("failed to create Bignum from it because of internal error") {
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("any BnUlong value") {
    big_num_should_fail_alloc = false;
    const BnUlong value = 1;

    WHEN("failed to create Bignum from it because of expansion error") {
      big_num_should_fail_one_time_set_word = true;
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("default created BigNum") {
    big_num_should_fail_alloc = false;
    const auto result = BigNum::New();

    WHEN("failed to convert to BnUlong because of internal error") {
      const auto converted = result.value().ToBnUlong();

      THEN("converted doesn't have value") { CHECK(!converted.has_value()); }
    }

    WHEN("failed to convert to dec") {
      const auto converted = result.value().ToDec();

      THEN("result doesn't have value") { CHECK(!converted.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    big_num_should_fail_alloc = false;
    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::New(Dec{"4"});

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    big_num_should_fail_alloc = false;
    const auto value = BigNum::New(15);

    WHEN("converting it to dec") {
      const auto result = value.value().ToHex();

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    big_num_should_fail_alloc = false;
    const auto value = BigNum::New(15);

    WHEN("converting it to bin") {
      const auto result = value.value().ToBin();

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("char pointer with number value") {
    big_num_should_fail_alloc = false;
    WHEN("failed to convert BigNum from it") {
      const auto result = BigNum::New(Hex{"0F"});

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  GIVEN("bin value of BigNum") {
    big_num_should_fail_alloc = false;
    SslData value{10};

    WHEN("failed to create BigNum from it") {
      const auto result = BigNum::New(value);

      THEN("result doesn't have value") { CHECK(!result.has_value()); }
    }
  }

  [&]() -> Result<void> {
    SECTION("add two BigNum failing") {
      big_num_should_fail_alloc = false;
      OUTCOME_TRY(const auto first, BigNum::New(2));
      OUTCOME_TRY(const auto second, BigNum::New(3));
      const auto result = BigNum::Add(first, second);
      CHECK(!result.has_value());
    }

    SECTION("BigNum += BigNum failing") {
      big_num_should_fail_alloc = false;

      OUTCOME_TRY(auto value, BigNum::New(2));
      OUTCOME_TRY(const auto second_value, BigNum::New(3));
      CHECK(!(value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK(value == result);
    }

    SECTION("BigNum += Result<BigNum> failing") {
      big_num_should_fail_alloc = false;

      OUTCOME_TRY(auto value, BigNum::New(2));
      CHECK(!(value += BigNum::New(3)).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK(value == result);
    }

    SECTION("Result<BigNum> += BigNum") {
      big_num_should_fail_alloc = false;

      auto value = BigNum::New(2);
      OUTCOME_TRY(const auto second_value, BigNum::New(3));
      CHECK(!(value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK(value.value() == result);
    }

    SECTION("Result<BigNum> += Result<BigNum>") {
      big_num_should_fail_alloc = false;

      auto value = BigNum::New(2);
      CHECK(!(value += BigNum::New(3)).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK(value.value() == result);
    }

    SECTION("!Result<BigNum> += Result<BigNum>") {
      big_num_should_fail_alloc = false;

      Result<BigNum> value = BigNumErrc::kAllocationFailure;
      CHECK(!(value += BigNum::New(3)).has_value());
      CHECK(!value.has_value());
    }

    SECTION("Result<BigNum> += !Result<BigNum>") {
      big_num_should_fail_alloc = false;

      auto value = BigNum::New(2);
      const Result<BigNum> second_value = BigNumErrc::kAllocationFailure;
      CHECK(!(value += second_value).has_value());
      OUTCOME_TRY(const auto result, BigNum::New(2));
      CHECK(value.value() == result);
    }
    return outcome_v2::success();
  }()
              .value();
}

}  // namespace glassy::test
