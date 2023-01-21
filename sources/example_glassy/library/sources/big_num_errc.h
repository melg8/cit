// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef BIG_NUM_ERRC_H
#define BIG_NUM_ERRC_H

#include <string>
#include <system_error>

#include <common_macro.h>

#include <outcome.hpp>

namespace glassy {
enum class BigNumErrc;
}

namespace std {
template <>
struct is_error_code_enum<glassy::BigNumErrc> : true_type {};
}  // namespace std

namespace glassy {
enum class BigNumErrc {
  kSuccess = 0,  // Zero should not represent an error.
  kAllocationFailure = 1,
  kNullPointerFailure = 2,
  kExpansionFailure = 3,
  kTooBigForConversion = 4,
  kConversionFailure = 5,
  kAdditionFailure = 6,
};

namespace detail {
struct BigNumErrorCategory : std::error_category {
  [[nodiscard]] const char* name() const noexcept override;
  [[nodiscard]] std::string message(int error_value) const override;
};

FORCEINLINE const char* BigNumErrorCategory::name() const noexcept {
  return "BigNum";
}

FORCEINLINE std::string BigNumErrorCategory::message(int error_value) const {
  switch (static_cast<BigNumErrc>(error_value)) {
    case BigNumErrc::kAllocationFailure:
      return "allocation failed";
    case BigNumErrc::kNullPointerFailure:
      return "got null pointer for creation of BigNum";
    case BigNumErrc::kExpansionFailure:
      return "failed due to unnecessary expansion";
    case BigNumErrc::kTooBigForConversion:
      return "value too big to fit";
    case BigNumErrc::kConversionFailure:
      return "failed to convert value";
    case BigNumErrc::kAdditionFailure:
      return "failed to add two values";
    case BigNumErrc::kSuccess:
      break;
  }
  return "successful";
}

// NOLINTNEXTLINE
const BigNumErrorCategory big_num_error_category{};
}  // namespace detail

// NOLINTNEXTLINE
FORCEINLINE std::error_code make_error_code(glassy::BigNumErrc e) {
  return std::error_code{static_cast<int>(e), detail::big_num_error_category};
}

}  // namespace glassy

#endif  // BIG_NUM_ERRC_H
