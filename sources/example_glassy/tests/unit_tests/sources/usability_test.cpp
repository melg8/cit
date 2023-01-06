// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_int.h>
#include <big_num.h>
#include <numeric_conversions.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

#define O_TRY OUTCOME_TRY

namespace glassy::test {

static Result<Asn1IntOwner> ProvideAsn1Value() noexcept {
  O_TRY(auto asn_1_int, Asn1IntegerFrom(32));
  O_TRY(auto bignum, convert::FromAsn1Int(asn_1_int));
  O_TRY(bignum += BigNum::New(1));
  O_TRY(convert::FromBigNum(bignum, asn_1_int));
  return {std::move(asn_1_int)};
}

SCENARIO("Asn1Int and Bignum usability") {
  const auto asn_1_int = ProvideAsn1Value();
  CHECK(asn_1_int.has_value());

  CHECK(std::is_eq(
      Asn1IntegerCmp(asn_1_int.value(), Asn1IntegerFrom(33).value())));
}

}  // namespace glassy::test
