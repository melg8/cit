// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <vector>

#include <doctest/doctest.h>
#include <outcome.hpp>

#include <asn_1_int_errc.h>  // NOLINT

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

struct Asn1IntErrcTestData {
  Asn1IntErrc error_code{};
  std::string message{};
};

SCENARIO("Asn1IntErrc names and messages") {
  auto tests = std::vector<Asn1IntErrcTestData>{
      {Asn1IntErrc::kSuccess, "successful"},
      {Asn1IntErrc::kAllocationFailure, "allocation failed"},
      {Asn1IntErrc::kNullPointerFailure,
       "got null pointer for creation of Asn1Int"},
      {Asn1IntErrc::kConversionFailure, "conversion failed"},
      {Asn1IntErrc::kCopyFailure, "copy failed"},
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

}  // namespace glassy::test
