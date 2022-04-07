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

    THEN("result value has zero bytes") {
      const auto number_of_bytes = result.value().NumberOfBytes();
      CHECK_EQ(number_of_bytes, 0);
    }

    WHEN("result converting to BnUlong") {
      const auto converted = BigNum::ToBnUlong(result.value());

      THEN("converted value equal to zero") { CHECK_EQ(converted.value(), 0); }
    }
  }

  GIVEN("zero BigNum value") {
    const auto result = BigNum::FromBnUlong(0).value();

    THEN("converted value equal to empty data") {
      const auto converted = BigNum::ToBin(result);
      SslData empty{};
      CHECK_EQ(converted.value(), empty);
    }

    THEN("result value has zero bytes") {
      const auto number_of_bytes = result.NumberOfBytes();
      CHECK_EQ(number_of_bytes, 0);
    }
  }

  GIVEN("any BnUlong value") {
    const BnUlong value = 1;

    WHEN("creating BigNum from it") {
      const auto result = BigNum::FromBnUlong(value);

      THEN("result has value") { CHECK(result.has_value()); }

      THEN("result value has proper number of bytes") {
        const auto number_of_bytes = result.value().NumberOfBytes();
        CHECK_EQ(number_of_bytes, 1);
      }

      WHEN("result converted back to BnUlong ") {
        const auto converted_back = BigNum::ToBnUlong(result.value());

        THEN("result has value") { CHECK(converted_back.has_value()); }

        THEN("converted back value equal to original value") {
          CHECK_EQ(converted_back.value(), value);
        }
      }

      WHEN("result converted to bin") {
        const auto converted = BigNum::ToBin(result.value());

        THEN("converted value equal to expected") {
          SslData expected{1};

          CHECK_EQ(converted.value(), expected);
        }
      }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    const auto value = BigNum::FromBnUlong(4).value();

    WHEN("converting it to dec") {
      const auto result = BigNum::ToDec(value);

      THEN("result has value") { CHECK(result.has_value()); }

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

      THEN("result has value") { CHECK(result.has_value()); }

      THEN("result BnUlong value is equal to expected") {
        const auto value = BigNum::ToBnUlong(result.value());

        CHECK_EQ(value.value(), 4);
      }
    }
  }

  GIVEN("BigNum created from BnUlongValue") {
    const auto value = BigNum::FromBnUlong(15).value();

    WHEN("converting it to dec") {
      const auto result = BigNum::ToHex(value);

      THEN("result has value") { CHECK(result.has_value()); }

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

      THEN("result has value") { CHECK(result.has_value()); }

      THEN("result BnUlong value is equal to expected") {
        const auto value = BigNum::ToBnUlong(result.value());

        CHECK_EQ(value.value(), 15);
      }
    }
  }

  GIVEN("bin value of BigNum") {
    SslData value{10};

    WHEN("creating BigNum from it") {
      const auto result = BigNum::FromBin(value);

      THEN("result has value") { CHECK(result.has_value()); }

      THEN("result BnUlong value is equal to expected") {
        const auto converted_value = BigNum::ToBnUlong(result.value()).value();
        CHECK_EQ(converted_value, 10);
      }

      THEN("result value has proper number of bytes") {
        const auto number_of_bytes = result.value().NumberOfBytes();
        CHECK_EQ(number_of_bytes, 1);
      }
    }
  }
}
