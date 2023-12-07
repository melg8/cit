// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_integer.h>
#include <big_num.h>
#include <numeric_conversions.h>

#include <testing_framework.h>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

SCENARIO("Asn1Integer conversions to/from BigNum") {
  [&]() -> Result<void> {
    {
      OUTCOME_TRY(const auto value, Asn1IntegerFrom(32));
      OUTCOME_TRY(const auto converted, convert::FromAsn1Int(value));
      CHECK(converted.ToBnUlong().value() == 32);
    }
    {
      OUTCOME_TRY(const auto value, BigNum::New(32));
      OUTCOME_TRY(const auto converted, convert::FromBigNum(value));
      CHECK(Asn1IntegerGet(converted).value() == 32);
    }
    {
      OUTCOME_TRY(const auto bignum_1, BigNum::New(32));

      OUTCOME_TRY(auto asn_1_int, Asn1IntegerFrom(10));
      OUTCOME_TRY(convert::FromBigNum(bignum_1, asn_1_int));
      OUTCOME_TRY(auto result_1, Asn1IntegerGet(asn_1_int));
      CHECK(result_1 == 32);

      OUTCOME_TRY(const auto bignum_2, BigNum::New(15));
      auto ptr = GetPtr(asn_1_int);
      OUTCOME_TRY(convert::FromBigNum(bignum_2, ptr));
      OUTCOME_TRY(auto result_2, Asn1IntegerGet(asn_1_int));
      CHECK(result_2 == 15);
    }

    return outcome::success();
  }()
               .value();
}

}  // namespace glassy::test
