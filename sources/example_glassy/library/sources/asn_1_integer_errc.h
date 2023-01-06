// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef ASN_1_INTEGER_ERRC_H
#define ASN_1_INTEGER_ERRC_H

#include <string>
#include <system_error>

#include <outcome.hpp>

namespace glassy {
enum class Asn1IntegerErrc;
}

namespace std {
template <>
struct is_error_code_enum<glassy::Asn1IntegerErrc> : true_type {};
}  // namespace std

namespace glassy {

enum class Asn1IntegerErrc {
  kSuccess = 0,  // 0 should not represent an error
  kAllocationFailure = 1,
  kNullPointerFailure = 2,
  kConversionFailure = 3,
  kCopyFailure = 4
};

namespace detail {
struct Asn1IntErrorCategory : std::error_category {
  [[nodiscard]] const char* name() const noexcept override;
  [[nodiscard]] std::string message(int error_value) const override;
};

inline const char* Asn1IntErrorCategory::name() const noexcept {
  return "BigNum";
}

inline std::string Asn1IntErrorCategory::message(int error_value) const {
  switch (static_cast<Asn1IntegerErrc>(error_value)) {
    case Asn1IntegerErrc::kAllocationFailure:
      return "allocation failed";
    case Asn1IntegerErrc::kNullPointerFailure:
      return "got null pointer for creation of Asn1Int";
    case Asn1IntegerErrc::kConversionFailure:
      return "conversion failed";
    case Asn1IntegerErrc::kCopyFailure:
      return "copy failed";
    case Asn1IntegerErrc::kSuccess:
      break;
  }
  return "successful";
}

// NOLINTNEXTLINE
const Asn1IntErrorCategory asn_1_int_error_category{};
}  // namespace detail

// NOLINTNEXTLINE
inline std::error_code make_error_code(glassy::Asn1IntegerErrc e) {
  return std::error_code{static_cast<int>(e), detail::asn_1_int_error_category};
}

}  // namespace glassy

#endif  // ASN_1_INTEGER_ERRC_H
