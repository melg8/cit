#include <doctest/doctest.h>

#include <bignum.h>

SCENARIO("BigNum") {
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

    WHEN("creating BigNum from it") {
      const auto result = BigNum::FromBnUlong(value);

      THEN("result is not empty") { CHECK(result); }

      THEN("result value has proper number of bytes") {
        const auto number_of_bytes = result.value().NumberOfBytes();
        CHECK_EQ(number_of_bytes, 1);
      }

      WHEN("result converted back to BnUlong ") {
        const auto converted_back = BigNum::ToBnUlong(result.value());

        THEN("result is not empty") { CHECK(converted_back); }

        THEN("converted back value equal to original value") {
          CHECK_EQ(converted_back.value(), value);
        }
      }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    const auto value = BigNum::FromBnUlong(4);

    WHEN("converting it to dec") {
      const auto result = BigNum::ToDec(value.value());

      THEN("result is not empty") { CHECK(result); }

      THEN("result value is equal to expected") {
        const std::string expected{"4"};

        CHECK_EQ(result.value().get(), expected);
      }
    }
  }

  GIVEN("char pointer with number value") {
    const auto* pointer = "4";

    WHEN("creating BigNum from it") {
      const auto result = BigNum::FromDec(pointer);

      THEN("result is not empty") { CHECK(result); }

      THEN("result BnUlong value is equal to expected") {
        const auto value = BigNum::ToBnUlong(result.value());

        CHECK_EQ(value.value(), 4);
      }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    const auto value = BigNum::FromBnUlong(15);

    WHEN("converting it to dec") {
      const auto result = BigNum::ToHex(value.value());

      THEN("result is not empty") { CHECK(result); }

      THEN("result value is equal to expected") {
        const std::string got{result.value().get()};
        CHECK_EQ(got, "0F");
      }
    }
  }

  GIVEN("char pointer with hex number value") {
    const auto* pointer = "0F";

    WHEN("creating BigNum from it") {
      const auto result = BigNum::FromHex(pointer);

      THEN("result is not empty") { CHECK(result); }

      THEN("result BnUlong value is equal to expected") {
        const auto value = BigNum::ToBnUlong(result.value());

        CHECK_EQ(value.value(), 15);
      }
    }
  }
}
