#include <big_num.h>

#include <doctest/doctest.h>

SCENARIO("BigNum can be included into multiple translations units and used") {
  GIVEN("default created BigNum") {
    const auto result = glassy::BigNum::New();

    THEN("result has value") { CHECK(result.has_value()); }
  }
}
