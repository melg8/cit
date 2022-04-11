#include <asn_1_int.h>
#include <numeric_conversions.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

namespace glassy {
namespace test {

using namespace OUTCOME_V2_NAMESPACE;

SCENARIO("Asn1Int conversion to BigNum") {
  const auto value = Asn1Int::New(32);
  const auto converted = convert::FromAsn1Int(value.value());
  CHECK(converted.has_value());
}

}  // namespace test
}  // namespace glassy
