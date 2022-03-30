#include <doctest/doctest.h>
#include <gsl/gsl-lite.hpp>

#include <bignum.h>

SCENARIO("gsl") {
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

SCENARIO("bignum") {
  GIVEN("two values") {
    const auto value_1 = 1;
    const auto value_2 = 2;

    WHEN("call Sum to add them") {
      const auto result = Sum(value_1, value_2);

      THEN("result is equal to expected") {
        const auto expected = 3;
        CHECK_EQ(result, expected);
      }
    }
  }
}
