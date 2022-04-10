#ifndef ASN_1_INT_ERRC_H
#define ASN_1_INT_ERRC_H

#include <outcome.hpp>

#include <system_error>

namespace glassy {
enum class Asn1IntErrc;
}

namespace std {
template <>
struct is_error_code_enum<glassy::Asn1IntErrc> : true_type {};
}  // namespace std

namespace glassy {

enum class Asn1IntErrc {
  Success = 0,  // 0 should not represent an error
  AllocationFailure = 1,
  ConversionFailure = 2,
};

namespace {
struct Asn1IntErrorCategory : std::error_category {
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

inline const char* Asn1IntErrorCategory::name() const noexcept {
  return "BigNum";
}

inline std::string Asn1IntErrorCategory::message(int ev) const {
  using namespace glassy;
  switch (static_cast<Asn1IntErrc>(ev)) {
    case Asn1IntErrc::Success:
      return "successful";
    case Asn1IntErrc::AllocationFailure:
      return "allocation failed";
    case Asn1IntErrc::ConversionFailure:
      return "conversion failed";
  }
  return "unknown";
}

const Asn1IntErrorCategory asn_1_int_error_category{};
}  // namespace

inline std::error_code make_error_code(glassy::Asn1IntErrc e) {
  return std::error_code{static_cast<int>(e), asn_1_int_error_category};
}

}  // namespace glassy

#endif  // ASN_1_INT_ERRC_H
