#include <doctest/doctest.h>
#include <outcome.hpp>

#include <bignum.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace glassy {
namespace test {

SCENARIO("sum function working") { CHECK_EQ(glassy::Sum(1, 2), 3); }

struct BigNumTestData {
  std::string subcase_name{};
  std::function<Result<BigNum>()> create{};
  int number_of_bytes{0};
  BnUlong value{0};
  std::string dec{};
  std::string hex{};
  SslData bin_data{};
};

#define CALL(X) []() { return BigNum::X; }

SCENARIO("BigNum creation and conversions") {
  using namespace OUTCOME_V2_NAMESPACE;
  const auto tests = std::vector<BigNumTestData>{
      {"default number", CALL(New()), 0, 0, "0", "0", SslData{}},
      {"from 0 number", CALL(FromBnUlong(0)), 0, 0, "0", "0", SslData{}},
      {"from 32 number", CALL(FromBnUlong(32)), 1, 32, "32", "20", SslData{32}},
      {"from dec", CALL(FromDec("15")), 1, 15, "15", "0F", SslData{15}},
      {"from hex", CALL(FromHex("0F")), 1, 15, "15", "0F", SslData{15}},
      {"from bin", CALL(FromBin(SslData{10})), 1, 10, "10", "0A", SslData{10}},
      {"from 2 byte bin", CALL(FromBin(SslData{255, 10})), 2, 65290, "65290",
       "FF0A", SslData{255, 10}}};
  std::for_each(
      std::begin(tests), std::end(tests), [](auto test) -> Result<void> {
        DOCTEST_SUBCASE(test.subcase_name.c_str()) {
          OUTCOME_TRY(const auto number, test.create());
          CHECK_EQ(number.NumberOfBytes(), test.number_of_bytes);
          CHECK_EQ(BigNum::ToBnUlong(number).value(), test.value);
          CHECK_EQ(std::string{BigNum::ToDec(number).value().get()}, test.dec);
          CHECK_EQ(std::string{BigNum::ToHex(number).value().get()}, test.hex);
          CHECK_EQ(BigNum::ToBin(number).value(), test.bin_data);
        }

        const auto x = test.subcase_name + " with >>= ";
        DOCTEST_SUBCASE(x.c_str()) {
          const auto number = test.create();
          CHECK_EQ(number.value().NumberOfBytes(), test.number_of_bytes);
          CHECK_EQ((number >>= BigNum::ToBnUlong).value(), test.value);
          CHECK_EQ(std::string{(number >>= BigNum::ToDec).value().get()},
                   test.dec);
          CHECK_EQ(std::string{(number >>= BigNum::ToHex).value().get()},
                   test.hex);
          CHECK_EQ((number >>= BigNum::ToBin).value(), test.bin_data);
        }
        return success();
      });
}

SCENARIO("failure result value propagates") {
  Result<BigNum> result_value{BigNumErrc::AllocationFailure};
  const auto result = result_value >>= BigNum::ToDec;
  CHECK_EQ(result.error(), std::error_code{BigNumErrc::AllocationFailure});
}
}  // namespace test
}  // namespace glassy
