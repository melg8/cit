#ifndef BIGNUM_H
#define BIGNUM_H

#include <openssl/bn.h>

#include <outcome.hpp>

#include <limits>
#include <memory>

namespace glassy {

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

class BigNum {
  struct Deleter {
    void operator()(BIGNUM* number) noexcept;
  };

  using BigNumImpl = std::unique_ptr<BIGNUM, Deleter>;

 public:
  static Result<BigNum> New() noexcept;
  static Result<BigNum> FromBnUlong(BnUlong value) noexcept;
  static Result<BnUlong> ToBnUlong(const BigNum& value) noexcept;

 private:
  explicit BigNum(BigNumImpl ptr) noexcept;

  BigNumImpl ptr_{};
};

using Error = decltype(Result<BigNum>::error_type{});

inline void glassy::BigNum::Deleter::operator()(BIGNUM* number) noexcept {
  BN_free(number);
}

inline BigNum::BigNum(BigNumImpl ptr) noexcept : ptr_{std::move(ptr)} {}

inline Result<BigNum> BigNum::New() noexcept {
  BigNumImpl ptr{BN_new()};
  if (!ptr) {
    return Error{};
  }

  return BigNum{std::move(ptr)};
}

inline Result<BigNum> BigNum::FromBnUlong(BnUlong value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_set_word(result.ptr_.get(), value) == 0) {
    return Error{};
  }
  return result;
}

inline Result<BnUlong> BigNum::ToBnUlong(const BigNum& value) noexcept {
  const auto result = BN_get_word(value.ptr_.get());
  if (result == std::numeric_limits<decltype(result)>::max()) {
    return Error{};
  }
  return result;
}

inline int Sum(int a, int b) { return a + b; }

}  // namespace glassy

#endif  // BIGNUM_H
