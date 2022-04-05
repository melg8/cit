#include <doctest/doctest.h>

#include <bignum.h>

SCENARIO("bignum") {
  using namespace glassy;

  GIVEN("two values") {
    const auto value_1 = 1;
    const auto value_2 = 2;

    WHEN("call Sum to add them") {
      const auto result = glassy::Sum(value_1, value_2);

      THEN("result is equal to expected") {
        const auto expected = 3;
        CHECK_EQ(result, expected);
      }
    }
  }

  GIVEN("default created BigNum") {
    const auto result = BigNum::New();
    THEN("result has value") { CHECK(result.has_value()); }

    WHEN("result converting to BnUlong") {
      const auto converted = BigNum::ToBnUlong(result.value());

      THEN("converted value equal to zero") { CHECK_EQ(converted.value(), 0); }
    }
  }

  GIVEN("any BnUlong value") {
    const BnUlong value = 1;

    WHEN("creating Bignum from it") {
      const auto result = BigNum::FromBnUlong(value);

      THEN("result is not empty") { CHECK(result); }

      WHEN("result converted back to BnUlong ") {
        const auto converted_back = BigNum::ToBnUlong(result.value());

        THEN("result is not empty") { CHECK(converted_back); }

        THEN("converted back value equal to original value") {
          CHECK_EQ(converted_back.value(), value);
        }
      }
    }
  }
}
