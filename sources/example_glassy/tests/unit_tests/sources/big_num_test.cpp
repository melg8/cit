// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <functional>
#include <vector>

#include <big_num.h>

#include <testing_framework.h>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

SCENARIO("sum function working") { CHECK(Sum(1, 2) == 3); }

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

#define CALL(X) []() noexcept { return BigNum::X; }

SCENARIO("BigNum creation and conversions") {
  auto tests = std::vector<BigNumTestData>{
      {"default number from pointer", CALL(Own(BN_new())), 0, 0, 0, "0", "0",
       SslData{}},
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
        SECTION(test.subcase_name.c_str()) {
          OUTCOME_TRY(const auto number, test.create());
          CHECK(number.NumberOfBytes() == test.number_of_bytes);
          CHECK(number.NumberOfBits() == test.number_of_bits);
          CHECK(number.ToBnUlong().value() == test.value);
          CHECK(std::string{number.ToDec().value().get()} == test.dec);
          CHECK(std::string{number.ToHex().value().get()} == test.hex);
          CHECK(number.ToBin().value() == test.bin_data);
        }
        return outcome::success();
      });
}

SCENARIO("BigNum operations") {
  []() -> Result<void> {
    SECTION("add two BigNum together") {
      OUTCOME_TRY(const auto first, BigNum::New(2));
      OUTCOME_TRY(const auto second, BigNum::New(3));
      OUTCOME_TRY(const auto added, BigNum::Add(first, second));
      OUTCOME_TRY(const auto result, added.ToBnUlong());
      CHECK(result == 5);
    }

    SECTION("add assign BigNum& to const BigNum&") {
      OUTCOME_TRY(auto first, BigNum::New(2));
      OUTCOME_TRY(const auto second, BigNum::New(3));
      OUTCOME_TRYV(first += second);

      OUTCOME_TRY(const auto result1, first.ToBnUlong());
      CHECK(result1 == 5);
    }

    SECTION("add assign BigNum& to const Result<BigNum>&") {
      OUTCOME_TRY(auto first, BigNum::New(2));
      const auto second = BigNum::New(3);
      OUTCOME_TRYV(first += second);

      OUTCOME_TRY(const auto result1, first.ToBnUlong());
      CHECK(result1 == 5);
    }

    SECTION("add assign BigNum& to Result<BigNum>&&") {
      OUTCOME_TRY(auto first, BigNum::New(2));
      OUTCOME_TRYV(first += BigNum::New(3) + BigNum::New(1));

      OUTCOME_TRY(const auto result1, first.ToBnUlong());
      CHECK(result1 == 6);
    }

    SECTION("add assign Result<BigNum>& to const BigNum&") {
      auto first = BigNum::New(2);
      OUTCOME_TRY(const auto second, BigNum::New(3));
      OUTCOME_TRYV(first += second);

      OUTCOME_TRY(const auto result1, first.value().ToBnUlong());
      CHECK(result1 == 5);
    }

    SECTION("add assign Result<BigNum>& to const Result<BigNum>&") {
      auto first = BigNum::New(2);
      const auto second = BigNum::New(3);
      OUTCOME_TRYV(first += second);

      OUTCOME_TRY(const auto result1, first.value().ToBnUlong());
      CHECK(result1 == 5);
    }

    SECTION("add assign Result<BigNum>& to Result<BigNum>&&") {
      auto first = BigNum::New(2);
      OUTCOME_TRYV(first += BigNum::New(3));

      OUTCOME_TRY(const auto result1, first.value().ToBnUlong());
      CHECK(result1 == 5);
    }

    SECTION("add several BigNum together") {
      const auto maybe_result = BigNum::New(1) + BigNum::New(Dec{"15"}) +
                                BigNum::New(Hex{"0F"}) +
                                BigNum::New(SslData{11});
      CHECK(maybe_result.value().ToBnUlong().value() == 42);

      CHECK((BigNum::New(1).value() + BigNum::New(Dec{"15"}))
                .value()
                .ToBnUlong()
                .value() == 16);
      CHECK((BigNum::New(1) + BigNum::New(Dec{"15"}).value())
                .value()
                .ToBnUlong()
                .value() == 16);
    }

    return outcome::success();
  }()
              .value();
}

SCENARIO("BigNum comparison") {
  SECTION("compare two BigNum values") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto zero, BigNum::New(0));
      OUTCOME_TRY(const auto one, BigNum::New(1));
      SECTION("compare") {
        CHECK(Compare(zero, zero) == 0);
        CHECK(Compare(one, one) == 0);
        CHECK(Compare(zero, one) == -1);
        CHECK(Compare(one, zero) == 1);
      }
      SECTION("less") {
        CHECK(zero < one);
        CHECK(!(one < zero));
        CHECK(!(zero < zero));
        CHECK(!(one < one));
      }
      SECTION("greater") {
        CHECK(one > zero);
        CHECK(!(zero > one));
        CHECK(!(zero > zero));
        CHECK(!(one > one));
      }
      SECTION("equal") {
        CHECK(zero == zero);
        CHECK(one == one);
        CHECK(!(zero == one));
        CHECK(!(one == zero));
      }
      SECTION("not equal") {
        CHECK(zero != one);
        CHECK(one != zero);
        CHECK(!(zero != zero));
        CHECK(!(one != one));
      }
      return outcome::success();
    }()
                .value();
  }
}

SCENARIO("BigNum creation") {
  SECTION("creation from nullptr should fail") {
    CHECK(!BigNum::Own(nullptr).has_value());
  }
}

}  // namespace glassy::test
