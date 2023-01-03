// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_int.h>
#include <big_num.h>
#include <numeric_conversions.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

static Asn1IntView ViewExtractor(Asn1IntOwner& owner) noexcept {
  return {owner};
}

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
      CHECK_EQ(ToLong(converted).value(), 32);
    }
    {
      OUTCOME_TRY(const auto bignum, BigNum::New(32));

      OUTCOME_TRY(auto asn_1_int, Asn1Int::New(10));
      OUTCOME_TRY(convert::FromBigNum(bignum, asn_1_int));

      auto view = ViewExtractor(asn_1_int);
      OUTCOME_TRY(convert::FromBigNum(bignum, view));
      CHECK_EQ(ToLong(asn_1_int).value(), 32);
    }

    return outcome::success();
  }()
              .value();
}

}  // namespace glassy::test
