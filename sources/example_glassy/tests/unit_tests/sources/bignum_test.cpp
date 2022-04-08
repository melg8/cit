#include <doctest/doctest.h>
#include <outcome.hpp>

#include <bignum.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace glassy {
namespace test {
using namespace OUTCOME_V2_NAMESPACE;

SCENARIO("sum function working") { CHECK_EQ(Sum(1, 2), 3); }

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
  const auto tests = std::vector<BigNumTestData>{
      {"default number", CALL(New()), 0, 0, 0, "0", "0", SslData{}},
      {"from 0 number", CALL(New(0)), 0, 0, 0, "0", "0", SslData{}},
      {"from 32 number", CALL(New(32)), 1, 6, 32, "32", "20", SslData{32}},
      {"from dec", CALL(New(Dec{"15"})), 1, 4, 15, "15", "0F", SslData{15}},
      {"from hex", CALL(New(Hex{"0F"})), 1, 4, 15, "15", "0F", SslData{15}},
      {"from bin", CALL(New(SslData{10})), 1, 4, 10, "10", "0A", SslData{10}},
      {"from 2 byte bin", CALL(New(SslData{255, 10})), 2, 16, 65290, "65290",
       "FF0A", SslData{255, 10}}};
  std::for_each(
      std::begin(tests), std::end(tests), [](auto test) -> Result<void> {
        DOCTEST_SUBCASE(test.subcase_name.c_str()) {
          OUTCOME_TRY(const auto number, test.create());
          CHECK_EQ(number.NumberOfBytes(), test.number_of_bytes);
          CHECK_EQ(number.NumberOfBits(), test.number_of_bits);
          CHECK_EQ(number.ToBnUlong().value(), test.value);
          CHECK_EQ(std::string{number.ToDec().value().get()}, test.dec);
          CHECK_EQ(std::string{number.ToHex().value().get()}, test.hex);
          CHECK_EQ(number.ToBin().value(), test.bin_data);
        }
        return success();
      });
}

SCENARIO("BigNum operations") {
  []() -> Result<void> {
    DOCTEST_SUBCASE("add two BigNum together") {
      OUTCOME_TRY(const auto first, BigNum::New(2));
      OUTCOME_TRY(const auto second, BigNum::New(3));
      OUTCOME_TRY(const auto added, BigNum::Add(first, second));
      OUTCOME_TRY(const auto result, added.ToBnUlong());
      CHECK_EQ(result, 5);
    }

    DOCTEST_SUBCASE("add several BigNum together") {
      const auto maybe_result = BigNum::New(1) + BigNum::New(Dec{"15"}) +
                                BigNum::New(Hex{"0F"}) +
                                BigNum::New(SslData{11});
      CHECK_EQ(maybe_result.value().ToBnUlong().value(), 42);

      CHECK_EQ((BigNum::New(1).value() + BigNum::New(Dec{"15"}))
                   .value()
                   .ToBnUlong()
                   .value(),
               16);
      CHECK_EQ((BigNum::New(1) + BigNum::New(Dec{"15"}).value())
                   .value()
                   .ToBnUlong()
                   .value(),
               16);
    }

    return success();
  }()
              .value();
}

}  // namespace test
}  // namespace glassy
