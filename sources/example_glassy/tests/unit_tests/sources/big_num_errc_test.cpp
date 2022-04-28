// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <vector>

#include <doctest/doctest.h>
#include <outcome.hpp>

#include <big_num_errc.h>  // NOLINT

namespace glassy {
namespace test {

namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

struct BigNumErrcTestData {
  BigNumErrc error_code{};
  std::string message{};
};

SCENARIO("Asn1IntErrc names and messages") {
  auto tests = std::vector<BigNumErrcTestData>{
      {BigNumErrc::kSuccess, "successful"},
      {BigNumErrc::kAllocationFailure, "allocation failed"},
      {BigNumErrc::kNullPointerFailure,
       "got null pointer for creation of BigNum"},
      {BigNumErrc::kExpansionFailure, "failed due to unnecessary expansion"},
      {BigNumErrc::kTooBigForConversion, "value too big to fit"},
      {BigNumErrc::kConversionFailure, "failed to convert value"},
      {BigNumErrc::kAdditionFailure, "failed to add two values"},
  };

  std::for_each(std::begin(tests), std::end(tests),
                [](auto test) -> Result<void> {
                  Result<void> result{test.error_code};
                  CHECK(result.has_failure());

                  CHECK_EQ(result.error().message(), test.message);
                  CHECK_EQ(result.error().category().name(), "BigNum");
                  return outcome::success();
                });
}

}  // namespace test
}  // namespace glassy
