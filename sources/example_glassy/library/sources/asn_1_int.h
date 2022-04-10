#ifndef ASN_1_INT_H
#define ASN_1_INT_H

#include <openssl/asn1.h>
#include <outcome.hpp>

#include <memory>
#include <utility>

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

using Long = long;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

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

class Asn1Int {
 public:
  static Result<Asn1Int> New(Long value = 0) noexcept;

  Result<Long> ToLong() const noexcept;

  const ASN1_INTEGER* Ptr() const noexcept;
  ASN1_INTEGER* Ptr() noexcept;

 private:
  struct Deleter {
    void operator()(ASN1_INTEGER* number) noexcept;
  };

  using Asn1IntImpl = std::unique_ptr<ASN1_INTEGER, Deleter>;

  static Result<Asn1Int> NewUninitialized() noexcept;

  explicit Asn1Int(Asn1IntImpl ptr) noexcept;

  Asn1IntImpl ptr_{};
};

inline void Asn1Int::Deleter::operator()(ASN1_INTEGER* number) noexcept {
  ASN1_INTEGER_free(number);
}

inline Result<Asn1Int> Asn1Int::NewUninitialized() noexcept {
  Asn1IntImpl ptr{ASN1_INTEGER_new()};
  if (!ptr) {
    return Asn1IntErrc::AllocationFailure;
  }
  return Asn1Int{std::move(ptr)};
}

inline glassy::Result<glassy::Asn1Int> glassy::Asn1Int::New(
    Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1Int::NewUninitialized());
  if (ASN1_INTEGER_set(result.Ptr(), value) == 0) {
    return Asn1IntErrc::AllocationFailure;
  }
  return result;
}

inline Result<Long> Asn1Int::ToLong() const noexcept {
  const auto result = ASN1_INTEGER_get(Ptr());
  if (result == -1) {
    return Asn1IntErrc::ConversionFailure;
  }
  return result;
}

inline const ASN1_INTEGER* Asn1Int::Ptr() const noexcept { return ptr_.get(); }

inline ASN1_INTEGER* Asn1Int::Ptr() noexcept { return ptr_.get(); }

inline Asn1Int::Asn1Int(Asn1IntImpl ptr) noexcept : ptr_{std::move(ptr)} {}

inline int Compare(const Asn1Int& lhs, const Asn1Int& rhs) noexcept {
  return ASN1_INTEGER_cmp(lhs.Ptr(), rhs.Ptr());
}

inline bool operator<(const Asn1Int& lhs, const Asn1Int& rhs) noexcept {
  return Compare(lhs, rhs) < 0;
}

inline bool operator>(const Asn1Int& lhs, const Asn1Int& rhs) noexcept {
  return Compare(lhs, rhs) > 0;
}

inline bool operator==(const Asn1Int& lhs, const Asn1Int& rhs) noexcept {
  return Compare(lhs, rhs) == 0;
}

inline bool operator!=(const Asn1Int& lhs, const Asn1Int& rhs) noexcept {
  return Compare(lhs, rhs) != 0;
}

}  // namespace glassy

#endif  // ASN_1_INT_H
