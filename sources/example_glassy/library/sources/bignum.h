#ifndef BIGNUM_H
#define BIGNUM_H

#include <openssl/bn.h>
#include <outcome.hpp>

#include <limits>
#include <memory>
#include <system_error>

namespace glassy {
enum class BigNumErrc;
}

namespace std {
template <>
struct std::is_error_code_enum<glassy::BigNumErrc> : true_type {};
}  // namespace std

namespace glassy {
enum class BigNumErrc {
  Success = 0,  // 0 should not represent an error
  AllocationFailure = 1,
  ExpansionFailure = 2,
  TooBigForConversion = 3
};

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

namespace {

struct BigNumErrorCategory : std::error_category {
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

inline const char* BigNumErrorCategory::name() const noexcept {
  return "BigNum";
}

inline std::string BigNumErrorCategory::message(int ev) const {
  using namespace glassy;
  switch (static_cast<BigNumErrc>(ev)) {
    case BigNumErrc::Success:
      return "successful";
    case BigNumErrc::AllocationFailure:
      return "allocation failed";
    case BigNumErrc::ExpansionFailure:
      return "failed due to unnecessary expansion";
    case BigNumErrc::TooBigForConversion:
      return "value too big to fit";
  }
}

const BigNumErrorCategory big_num_error_category{};
}  // namespace

inline std::error_code make_error_code(glassy::BigNumErrc e) {
  return std::error_code{static_cast<int>(e), big_num_error_category};
}

class BigNum {
 public:
  static Result<BigNum> New() noexcept;
  static Result<BigNum> FromBnUlong(BnUlong value) noexcept;
  static Result<BnUlong> ToBnUlong(const BigNum& value) noexcept;

 private:
  struct Deleter {
    void operator()(BIGNUM* number) noexcept;
  };

  using BigNumImpl = std::unique_ptr<BIGNUM, Deleter>;

  explicit BigNum(BigNumImpl ptr) noexcept;

  BigNumImpl ptr_{};
};

inline void glassy::BigNum::Deleter::operator()(BIGNUM* number) noexcept {
  BN_free(number);
}

inline BigNum::BigNum(BigNumImpl ptr) noexcept : ptr_{std::move(ptr)} {}

inline Result<BigNum> BigNum::New() noexcept {
  BigNumImpl ptr{BN_new()};
  if (!ptr) {
    return BigNumErrc::AllocationFailure;
  }

  return BigNum{std::move(ptr)};
}

inline Result<BigNum> BigNum::FromBnUlong(BnUlong value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_set_word(result.ptr_.get(), value) == 0) {
    return BigNumErrc::ExpansionFailure;
  }
  return result;
}

inline Result<BnUlong> BigNum::ToBnUlong(const BigNum& value) noexcept {
  const auto result = BN_get_word(value.ptr_.get());
  if (result == std::numeric_limits<decltype(result)>::max()) {
    return BigNumErrc::TooBigForConversion;
  }
  return result;
}

inline int Sum(int a, int b) { return a + b; }

}  // namespace glassy

#endif  // BIGNUM_H
