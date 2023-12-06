// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <string>
#include <vector>

#include <testing_framework.h>
#include <outcome.hpp>

#include <asn_1_integer_errc.h>  // NOLINT

namespace glassy::test {

namespace outcome = OUTCOME_V2_NAMESPACE;
using std::string_literals::operator""s;  // NOLINT(misc-unused-using-decls)

template <typename T>
using Result = outcome::result<T>;

struct Asn1IntegerErrcTestData {
  Asn1IntegerErrc error_code{};
  std::string message{};
};

SCENARIO("Asn1IntegerErrc names and messages") {
  auto tests = std::vector<Asn1IntegerErrcTestData>{
      {Asn1IntegerErrc::kSuccess, "successful"},
      {Asn1IntegerErrc::kAllocationFailure, "allocation failed"},
      {Asn1IntegerErrc::kNullPointerFailure,
       "got null pointer for creation of Asn1Int"},
      {Asn1IntegerErrc::kConversionFailure, "conversion failed"},
      {Asn1IntegerErrc::kCopyFailure, "copy failed"},
  };

  std::ranges::for_each(tests, [&](auto test) -> Result<void> {
    Result<void> result{test.error_code};
    CHECK(result.has_failure());

    CHECK(result.error().message() == test.message);
    CHECK(result.error().category().name() == "BigNum"s);
    return outcome::success();
  });
}

}  // namespace glassy::test
