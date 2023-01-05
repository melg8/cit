// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <functional>
#include <vector>

#include <asn_1_int.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

struct Asn1IntTestData {
  std::string subcase_name{};
  std::function<Result<Asn1IntOwner>()> create{};
  Long value{0};
};

#define CALL(X) []() noexcept { return glassy::X; }

SCENARIO("Asn1Int creation and conversions") {
  auto tests = std::vector<Asn1IntTestData>{
      {"number 0", CALL(Asn1IntegerFrom(0)), 0},
      {"number 32", CALL(Asn1IntegerFrom(32)), 32},
  };

  std::for_each(std::begin(tests), std::end(tests),
                [](auto test) -> Result<void> {
                  SUBCASE(test.subcase_name.c_str()) {
                    OUTCOME_TRY(const auto number, test.create());
                    CHECK_EQ(Asn1IntegerGet(number).value(), test.value);
                  }

                  return outcome::success();
                });
}

#define CHECK_IS_EQ(X) CHECK(std::is_eq(X))
#define RVALUE(X) Asn1IntegerFrom(X).value()

SCENARIO("Asn1Int comparison") {
  SUBCASE("compare two Asn1Int values") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto one, Asn1IntegerFrom(1));
      OUTCOME_TRY(const auto zero, Asn1IntegerFrom(0));
      SUBCASE("compare") {
        CHECK_IS_EQ(Asn1IntegerCmp(zero, zero));
        CHECK_IS_EQ(Asn1IntegerCmp(one, one));
        CHECK(std::is_lt(Asn1IntegerCmp(zero, one)));
        CHECK(std::is_gt(Asn1IntegerCmp(one, zero)));
      }

      SUBCASE("compare rvalue references") {
        CHECK_IS_EQ(Asn1IntegerCmp(RVALUE(0), RVALUE(0)));
        CHECK_IS_EQ(Asn1IntegerCmp(RVALUE(1), RVALUE(1)));
        CHECK(std::is_lt(Asn1IntegerCmp(RVALUE(0), RVALUE(1))));
        CHECK(std::is_gt(Asn1IntegerCmp(RVALUE(1), RVALUE(0))));
      }

      SUBCASE("compare lvalue references") {
        const auto& l_zero = zero;
        const auto& l_one = one;
        CHECK_IS_EQ(Asn1IntegerCmp(l_zero, l_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(l_one, l_one));
        CHECK(std::is_lt(Asn1IntegerCmp(l_zero, l_one)));
        CHECK(std::is_gt(Asn1IntegerCmp(l_one, l_zero)));
      }

      SUBCASE("compare not null const pointers") {
        Asn1IntegerConstNotNull ptr_zero = zero.get();
        Asn1IntegerConstNotNull ptr_one = one.get();
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_zero, ptr_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_one, ptr_one));
        CHECK(std::is_lt(Asn1IntegerCmp(ptr_zero, ptr_one)));
        CHECK(std::is_gt(Asn1IntegerCmp(ptr_one, ptr_zero)));
      }

      SUBCASE("compare not null pointers") {
        Asn1IntegerNotNull ptr_zero = zero.get();
        Asn1IntegerNotNull ptr_one = one.get();
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_zero, ptr_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_one, ptr_one));
        CHECK(std::is_lt(Asn1IntegerCmp(ptr_zero, ptr_one)));
        CHECK(std::is_gt(Asn1IntegerCmp(ptr_one, ptr_zero)));
      }

      return outcome::success();
    }()
                .value();
  }
}

SCENARIO("Asn1Int creation from pointer") {
  SUBCASE("creation from valid pointer should succeed") {
    CHECK(Own(ASN1_INTEGER_new()).has_value());
  }

  SUBCASE("creation from nullptr should fail") {
    CHECK_FALSE(Own(nullptr).has_value());
  }
}

SCENARIO("Asn1Int copy") {
  SUBCASE("create Asn1Int copy from original value") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto original, Asn1IntegerFrom(32));
      OUTCOME_TRY(const auto copy, Asn1IntegerDup(original));

      CHECK_NE(original.get(), copy.get());
      CHECK_EQ(Asn1IntegerGet(original).value(), Asn1IntegerGet(copy).value());

      CHECK_NE(original, copy);
      CHECK_EQ(Asn1IntegerGet(copy).value(), 32);

      return outcome::success();
    }()
                .value();
  }
}

static void TestFunction(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const ASN1_INTEGER* pointer = GetPtr(view);
  CHECK_NE(pointer, nullptr);
}

SCENARIO("ASn1IntConstView") {
  SUBCASE("create Asn1IntConstView from owners") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto const_owner, Asn1IntegerFrom(32));
      OUTCOME_TRY(auto mutable_owner, Asn1IntegerFrom(32));
      CHECK(std::is_eq(Asn1IntegerCmp(const_owner, mutable_owner)));

      const Asn1IntegerConstNotNull view_from_const_owner{GetPtr(const_owner)};
      const Asn1IntegerNotNull view_from_mutable_owner{GetPtr(mutable_owner)};
      CHECK(std::is_eq(
          Asn1IntegerCmp(view_from_const_owner, view_from_mutable_owner)));
      return outcome::success();
    }()
                .value();

    SUBCASE(
        "can use owned and pointers in function taking  Asn1IntConstView as "
        "arguments") {
      []() -> Result<void> {
        OUTCOME_TRY(const auto const_owner, Asn1IntegerFrom(32));
        TestFunction(const_owner);

        const Asn1IntegerConstNotNull const_pointer = const_owner.get();
        TestFunction(const_pointer);

        OUTCOME_TRY(auto mutable_owner, Asn1IntegerFrom(32));
        TestFunction(mutable_owner);

        const Asn1IntegerNotNull mutable_pointer = GetPtr(mutable_owner);
        TestFunction(mutable_pointer);

        return outcome::success();
      }()
                  .value();
    }
  }
}

}  // namespace glassy::test
