#include <asn_1_int.h>
#include <big_num.h>
#include <numeric_conversions.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

namespace glassy {
namespace test {

using namespace OUTCOME_V2_NAMESPACE;

SCENARIO("Asn1Int conversions to/from BigNum") {
  []() -> Result<void> {
    {
      OUTCOME_TRY(const auto value, Asn1Int::New(32));
      OUTCOME_TRY(const auto converted, convert::FromAsn1Int(value));
      CHECK_EQ(converted.ToBnUlong().value(), 32);
    }
    {
      OUTCOME_TRY(const auto value, BigNum::New(32));
      OUTCOME_TRY(const auto converted, convert::FromBigNum(value));
      CHECK_EQ(converted.ToLong().value(), 32);
    }

    return success();
  }()
              .value();
}

}  // namespace test
}  // namespace glassy