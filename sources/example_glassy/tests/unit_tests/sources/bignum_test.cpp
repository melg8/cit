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
  int number_of_bits{0};
  BnUlong value{0};
  std::string dec{};
  std::string hex{};
  SslData bin_data{};
};

#define CALL(X) []() { return BigNum::X; }

SCENARIO("BigNum creation and conversions") {
  using namespace OUTCOME_V2_NAMESPACE;
  const auto tests = std::vector<BigNumTestData>{
      {"default number", CALL(New()), 0, 0, 0, "0", "0", SslData{}},
      {"from 0 number", CALL(FromBnUlong(0)), 0, 0, 0, "0", "0", SslData{}},
      {"from 32 number", CALL(FromBnUlong(32)), 1, 6, 32, "32", "20",
       SslData{32}},
      {"from dec", CALL(FromDec("15")), 1, 4, 15, "15", "0F", SslData{15}},
      {"from hex", CALL(FromHex("0F")), 1, 4, 15, "15", "0F", SslData{15}},
      {"from bin", CALL(FromBin(SslData{10})), 1, 4, 10, "10", "0A",
       SslData{10}},
      {"from 2 byte bin", CALL(FromBin(SslData{255, 10})), 2, 16, 65290,
       "65290", "FF0A", SslData{255, 10}}};
  std::for_each(
      std::begin(tests), std::end(tests), [](auto test) -> Result<void> {
        DOCTEST_SUBCASE(test.subcase_name.c_str()) {
          OUTCOME_TRY(const auto number, test.create());
          CHECK_EQ(number.NumberOfBytes(), test.number_of_bytes);
          CHECK_EQ(number.NumberOfBits(), test.number_of_bits);
          CHECK_EQ(BigNum::ToBnUlong(number).value(), test.value);
          CHECK_EQ(std::string{BigNum::ToDec(number).value().get()}, test.dec);
          CHECK_EQ(std::string{BigNum::ToHex(number).value().get()}, test.hex);
          CHECK_EQ(BigNum::ToBin(number).value(), test.bin_data);
        }
        return success();
      });
}

}  // namespace test
}  // namespace glassy
