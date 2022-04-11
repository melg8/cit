#ifndef BIG_NUM_ERRC_H
#define BIG_NUM_ERRC_H

#include <outcome.hpp>

#include <system_error>

namespace glassy {
enum class BigNumErrc;
}

namespace std {
template <>
struct is_error_code_enum<glassy::BigNumErrc> : true_type {};
}  // namespace std

namespace glassy {
enum class BigNumErrc {
  Success = 0,  // 0 should not represent an error
  AllocationFailure = 1,
  ExpansionFailure = 2,
  TooBigForConversion = 3,
  ConversionFailure = 4,
  AdditionFailure = 5,
};

namespace {
struct BigNumErrorCategory : std::error_category {
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

inline const char* BigNumErrorCategory::name() const noexcept {
  return "BigNum";
}

inline std::string BigNumErrorCategory::message(int ev) const {
  switch (static_cast<BigNumErrc>(ev)) {
    case BigNumErrc::Success:
      return "successful";
    case BigNumErrc::AllocationFailure:
      return "allocation failed";
    case BigNumErrc::ExpansionFailure:
      return "failed due to unnecessary expansion";
    case BigNumErrc::TooBigForConversion:
      return "value too big to fit";
    case BigNumErrc::ConversionFailure:
      return "failed to convert value";
    case BigNumErrc::AdditionFailure:
      return "failed to add two values";
  }
  return "unknown";
}

const BigNumErrorCategory big_num_error_category{};
}  // namespace

inline std::error_code make_error_code(glassy::BigNumErrc e) {
  return std::error_code{static_cast<int>(e), big_num_error_category};
}

}  // namespace glassy

#endif  // BIG_NUM_ERRC_H
