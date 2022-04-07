#include <doctest/doctest.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

#include <openssl/bn.h>

SCENARIO("openssl") { BN_CTX_free(BN_CTX_new()); }
using namespace OUTCOME_V2_NAMESPACE;

static result<int> operator+(result<int> lhs, int rhs) noexcept {
  OUTCOME_TRY(auto result, lhs);
  return result + rhs;
}

SCENARIO("outcome") {
  using namespace OUTCOME_V2_NAMESPACE;

  GIVEN("some outcome with value") {
    result<int> test{10};

    WHEN("add outcome value to integer values") {
      const auto result = test + 5 + 10;

      THEN("it has value") { CHECK(result.has_value()); }

      THEN("it has expected value") { CHECK_EQ(result.value(), 25); }
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