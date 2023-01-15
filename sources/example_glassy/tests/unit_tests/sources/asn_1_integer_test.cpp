// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <functional>
#include <vector>

#include <asn_1_integer.h>

#include <testing_framework.h>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

struct Asn1IntegerTestData {
  std::string subcase_name{};
  std::function<Result<Asn1Integer>()> create{};
  Long value{0};
};

#define CALL(X) []() noexcept { return glassy::X; }

#define CHECK_IS_EQ(X) CHECK(std::is_eq(X))
#define CHECK_IS_LT(X) CHECK(std::is_lt(X))
#define CHECK_IS_GT(X) CHECK(std::is_gt(X))
#define RVALUE(X) Asn1IntegerFrom(X).value()

SCENARIO("Asn1Integer creation and conversions") {
  auto tests = std::vector<Asn1IntegerTestData>{
      {"number 0", CALL(Asn1IntegerFrom(0)), 0},
      {"number 32", CALL(Asn1IntegerFrom(32)), 32},
  };

  std::for_each(std::begin(tests), std::end(tests),
                [](auto test) -> Result<void> {
                  SECTION(test.subcase_name.c_str()) {
                    OUTCOME_TRY(const auto number, test.create());
                    CHECK(Asn1IntegerGet(number).value() == test.value);
                  }

                  return outcome::success();
                });
}

SCENARIO("Asn1Integer comparison") {
  SECTION("compare two Asn1Integer values") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto one, Asn1IntegerFrom(1));
      OUTCOME_TRY(const auto zero, Asn1IntegerFrom(0));
      SECTION("compare") {
        CHECK_IS_EQ(Asn1IntegerCmp(zero, zero));
        CHECK_IS_EQ(Asn1IntegerCmp(one, one));
        CHECK_IS_LT(Asn1IntegerCmp(zero, one));
        CHECK_IS_GT(Asn1IntegerCmp(one, zero));
      }

      SECTION("compare value of") {
        CHECK_IS_EQ(ValueOf(zero) <=> ValueOf(zero));
        CHECK(ValueOf(zero) == ValueOf(zero));

        CHECK(ValueOf(zero) == ValueOf(zero));
        CHECK(ValueOf(one) == ValueOf(one));
        CHECK(ValueOf(zero) < ValueOf(one));
        CHECK(ValueOf(one) > ValueOf(zero));
      }

      SECTION("compare rvalue references") {
        CHECK_IS_EQ(Asn1IntegerCmp(RVALUE(0), RVALUE(0)));
        CHECK_IS_EQ(Asn1IntegerCmp(RVALUE(1), RVALUE(1)));
        CHECK_IS_LT(Asn1IntegerCmp(RVALUE(0), RVALUE(1)));
        CHECK_IS_GT(Asn1IntegerCmp(RVALUE(1), RVALUE(0)));
      }

      SECTION("compare value of rvalue references") {
        ValueOf(Asn1IntegerFrom(0).value());

        const auto zero_equals = ValueOf(RVALUE(0)) == ValueOf(RVALUE(0));
        CHECK(zero_equals);

        const auto one_equals = ValueOf(RVALUE(1)) == ValueOf(RVALUE(1));
        CHECK(one_equals);

        const auto one_not_equal_zero =
            ValueOf(RVALUE(0)) != ValueOf(RVALUE(1));
        CHECK(one_not_equal_zero);

        // It owns lifetime of object if gets rvalue.
        auto lhs_cmp = ValueOf(RVALUE(0));
        auto rhs_cmp = ValueOf(RVALUE(1));
        CHECK(lhs_cmp < rhs_cmp);
        CHECK(rhs_cmp > lhs_cmp);

        const auto greater = ValueOf(RVALUE(1)) > ValueOf(RVALUE(0));
        CHECK(greater);

        const auto less = ValueOf(RVALUE(0)) < ValueOf(RVALUE(1));
        CHECK(less);
      }

      SECTION("compare lvalue references") {
        const auto& l_zero = zero;
        const auto& l_one = one;
        CHECK_IS_EQ(Asn1IntegerCmp(l_zero, l_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(l_one, l_one));
        CHECK_IS_LT(Asn1IntegerCmp(l_zero, l_one));
        CHECK_IS_GT(Asn1IntegerCmp(l_one, l_zero));
      }

      SECTION("compare value of different presentations") {
        auto lhs_cmp = ValueOf(RVALUE(0));
        const auto equals_left_value = ValueOf(RVALUE(0)) == lhs_cmp;
        CHECK(equals_left_value);

        const auto equals_right_value = lhs_cmp == ValueOf(RVALUE(0));
        CHECK(equals_right_value);

        const auto& l_zero = zero;

        const auto equals_left_reference =
            ValueOf(RVALUE(0)) == ValueOf(l_zero);
        CHECK(equals_left_reference);

        const auto equals_right_reference =
            ValueOf(l_zero) == ValueOf(RVALUE(0));
        CHECK(equals_right_reference);

        Asn1IntegerConstNotNull const_ptr_zero = zero.get();

        const auto equals_left_const_not_null =
            ValueOf(RVALUE(0)) == ValueOf(const_ptr_zero);
        CHECK(equals_left_const_not_null);

        const auto equals_right_const_not_null =
            ValueOf(const_ptr_zero) == ValueOf(RVALUE(0));

        CHECK(equals_right_const_not_null);

        Asn1IntegerNotNull ptr_zero = zero.get();

        const auto equals_left_not_null =
            ValueOf(RVALUE(0)) == ValueOf(ptr_zero);
        CHECK(equals_left_not_null);

        const auto equals_right_not_null =
            ValueOf(ptr_zero) == ValueOf(RVALUE(0));
        CHECK(equals_right_not_null);

        CHECK(ValueOf(const_ptr_zero) == ValueOf(ptr_zero));
        CHECK(ValueOf(const_ptr_zero) == ValueOf(ptr_zero));
      }

      SECTION("compare value of lvalue references") {
        const auto& l_zero = zero;
        const auto& l_one = one;

        CHECK(ValueOf(l_zero) == ValueOf(l_zero));
        CHECK(ValueOf(l_one) == ValueOf(l_one));
        CHECK(ValueOf(l_zero) < ValueOf(l_one));
        CHECK(ValueOf(l_one) > ValueOf(l_zero));
      }

      SECTION("compare not null const pointers") {
        Asn1IntegerConstNotNull ptr_zero = zero.get();
        Asn1IntegerConstNotNull ptr_one = one.get();
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_zero, ptr_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_one, ptr_one));
        CHECK_IS_LT(Asn1IntegerCmp(ptr_zero, ptr_one));
        CHECK_IS_GT(Asn1IntegerCmp(ptr_one, ptr_zero));
      }

      SECTION("compare value of not null const pointers") {
        Asn1IntegerConstNotNull ptr_zero = zero.get();
        Asn1IntegerConstNotNull ptr_one = one.get();
        CHECK(ValueOf(ptr_zero) == ValueOf(ptr_zero));
        CHECK(ValueOf(ptr_one) == ValueOf(ptr_one));
        CHECK(ValueOf(ptr_zero) < ValueOf(ptr_one));
        CHECK(ValueOf(ptr_one) > ValueOf(ptr_zero));
      }

      SECTION("compare not null pointers") {
        Asn1IntegerNotNull ptr_zero = zero.get();
        Asn1IntegerNotNull ptr_one = one.get();
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_zero, ptr_zero));
        CHECK_IS_EQ(Asn1IntegerCmp(ptr_one, ptr_one));
        CHECK_IS_LT(Asn1IntegerCmp(ptr_zero, ptr_one));
        CHECK_IS_GT(Asn1IntegerCmp(ptr_one, ptr_zero));
      }

      SECTION("compare value of not null pointers") {
        Asn1IntegerNotNull ptr_zero = zero.get();
        Asn1IntegerNotNull ptr_one = one.get();
        CHECK(ValueOf(ptr_zero) == ValueOf(ptr_zero));
        CHECK(ValueOf(ptr_one) == ValueOf(ptr_one));
        CHECK(ValueOf(ptr_zero) < ValueOf(ptr_one));
        CHECK(ValueOf(ptr_one) > ValueOf(ptr_zero));
      }
      return outcome::success();
    }()
                .value();
  }
}

SCENARIO("Asn1Integer creation from pointer") {
  SECTION("creation from valid pointer should succeed") {
    CHECK(Own(ASN1_INTEGER_new()).has_value());
  }

  SECTION("creation from nullptr should fail") {
    CHECK(!Own(nullptr).has_value());
  }
}

SCENARIO("Asn1Integer copy") {
  SECTION("create Asn1Integer copy from original value") {
    []() -> Result<void> {
      OUTCOME_TRY(const auto original, Asn1IntegerFrom(32));
      OUTCOME_TRY(const auto copy, Asn1IntegerDup(original));

      CHECK(original.get() != copy.get());
      CHECK(Asn1IntegerGet(original).value() == Asn1IntegerGet(copy).value());

      CHECK(original != copy);
      CHECK(Asn1IntegerGet(copy).value() == 32);

      return outcome::success();
    }()
                .value();
  }
}

static void TestFunction(
    not_null_provider_of<const ASN1_INTEGER*> auto&& view) noexcept {
  const ASN1_INTEGER* pointer = GetPtr(view);
  CHECK(pointer != nullptr);
}

SCENARIO("Asn1IntegerConstView") {
  SECTION("create Asn1IntegerConstView from owners") {
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

    SECTION(
        "can use owned and pointers in function taking  Asn1IntegerConstView "
        "as arguments") {
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

    SECTION("can set new value to already created Asn1Integer") {
      []() -> Result<void> {
        {
          OUTCOME_TRY(auto value, Asn1IntegerFrom(0));
          OUTCOME_TRY(Asn1IntegerSet(value, 32));
          CHECK_IS_EQ(Asn1IntegerCmp(value, Asn1IntegerFrom(32).value()));
        }
        {
          OUTCOME_TRY(auto value, Asn1IntegerFrom(0));
          auto not_null_ptr = GetPtr(value);
          OUTCOME_TRY(Asn1IntegerSet(not_null_ptr, 32));
          CHECK_IS_EQ(Asn1IntegerCmp(value, Asn1IntegerFrom(32).value()));
        }

        return outcome::success();
      }()
                  .value();
    }
  }
}

}  // namespace glassy::test
