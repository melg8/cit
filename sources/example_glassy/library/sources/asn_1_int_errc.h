#ifndef ASN_1_INT_ERRC_H
#define ASN_1_INT_ERRC_H

#include <system_error>

#include <outcome.hpp>

namespace glassy {
enum class Asn1IntErrc;
}

namespace std {
template <>
struct is_error_code_enum<glassy::Asn1IntErrc> : true_type {};
}  // namespace std

namespace glassy {

enum class Asn1IntErrc {
  kSuccess = 0,  // 0 should not represent an error
  kAllocationFailure = 1,
  kNullPointerFailure = 2,
  kConversionFailure = 3,
  kCopyFailure = 4
};

namespace detail {
struct Asn1IntErrorCategory : std::error_category {
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

inline const char* Asn1IntErrorCategory::name() const noexcept {
  return "BigNum";
}

inline std::string Asn1IntErrorCategory::message(int ev) const {
  switch (static_cast<Asn1IntErrc>(ev)) {
    case Asn1IntErrc::kSuccess:
      return "successful";
    case Asn1IntErrc::kAllocationFailure:
      return "allocation failed";
    case Asn1IntErrc::kNullPointerFailure:
      return "got null pointer for creation of Asn1Int";
    case Asn1IntErrc::kConversionFailure:
      return "conversion failed";
    case Asn1IntErrc::kCopyFailure:
      return "copy failed";
  }
  return "unknown";
}

// NOLINTNEXTLINE
const Asn1IntErrorCategory asn_1_int_error_category{};
}  // namespace detail

// NOLINTNEXTLINE
inline std::error_code make_error_code(glassy::Asn1IntErrc e) {
  return std::error_code{static_cast<int>(e), detail::asn_1_int_error_category};
}

}  // namespace glassy

#endif  // ASN_1_INT_ERRC_H
