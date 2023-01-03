// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <doctest/doctest.h>
#include <openssl/bn.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

SCENARIO("openssl") { BN_CTX_free(BN_CTX_new()); }

static outcome::result<int> operator+(outcome::result<int> lhs,
                                      int rhs) noexcept {
  OUTCOME_TRY(auto result, lhs);
  return result + rhs;
}

SCENARIO("outcome") {
  GIVEN("some outcome with value") {
    outcome::result<int> test{1};

    WHEN("add outcome value to integer values") {
      const auto result = test + 5 + 10;

      THEN("it has value") { CHECK(result.has_value()); }

      THEN("it has expected value") { CHECK_EQ(result.value(), 16); }
    }
  }
}

SCENARIO("gsl-lite") {
  GIVEN("uint64_t value with small value") {
    const uint64_t value = 42;

    WHEN("converting it to uint8_t") {
      const auto result = gsl::narrow<uint8_t>(value);

      THEN("result is narrowed correctly and equal to expected") {
        CHECK_EQ(result, uint8_t{42});
      }
    }
  }
}

}  // namespace glassy::test
