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

  GIVEN("any BnUlong value") {
    const BnUlong value = 1;

    WHEN("creating Bignum from it") {
      const auto result = BigNum::fromBnUlong(value);

      THEN("result is not empty") { CHECK(result); }

      THEN("result converted back to BnUlong equal to original value") {
        //        const auto converted_back = BigNum::ToUlong(*result);

        //        CHECK_EQ(converted_back, value);
      }
    }
  }
}
