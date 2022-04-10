#include <doctest/doctest.h>
#include <outcome.hpp>

#include <asn_1_int.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace glassy {
namespace test {

using namespace OUTCOME_V2_NAMESPACE;

struct Asn1IntTestData {
  std::string subcase_name{};
  std::function<Result<Asn1Int>()> create{};
  Long value{0};
};

#define CALL(X) []() { return Asn1Int::X; }

SCENARIO("Asn1Int creation and conversions") {
  const auto tests = std::vector<Asn1IntTestData>{
      {"default number", CALL(New()), 0},
      {"default number", CALL(New(0)), 0},
      {"default number", CALL(New(32)), 32},
  };

  std::for_each(std::begin(tests), std::end(tests),
                [](auto test) -> Result<void> {
                  SUBCASE(test.subcase_name.c_str()) {
                    OUTCOME_TRY(const auto number, test.create());
                    CHECK_EQ(number.ToLong().value(), test.value);
                  }

                  return success();
                });
}

}  // namespace test
}  // namespace glassy
