#ifndef BIG_NUM_H
#define BIG_NUM_H

#include <big_num_errc.h>

#include <openssl/bn.h>
#include <openssl/engine.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

#include <limits>
#include <memory>
#include <utility>
#include <vector>

namespace glassy {

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

struct OpenSslFree {
  template <typename T>
  void operator()(T* ptr) noexcept {
    OPENSSL_free(ptr);
  }  // namespace glassy
};

using SslString = std::unique_ptr<char, OpenSslFree>;
using SslData = std::vector<unsigned char>;  // TODO(melg): replace with openssl
                                             // based allocation mechanism.
using SslSpan = gsl::span<const unsigned char>;

struct Dec {
  const char* const value{nullptr};
};

struct Hex {
  const char* const value{nullptr};
};

class BigNum {
 public:
  static Result<BigNum> New() noexcept;
  static Result<BigNum> New(BnUlong value) noexcept;
  static Result<BigNum> New(const Dec& dec) noexcept;
  static Result<BigNum> New(const Hex& hex) noexcept;
  static Result<BigNum> New(const SslSpan& span) noexcept;
  static Result<BigNum> Own(BIGNUM* ptr) noexcept;

  static Result<BigNum> Add(const BigNum& lhs, const BigNum& rhs) noexcept;

  const BIGNUM* Ptr() const noexcept;
  Result<BnUlong> ToBnUlong() const noexcept;
  Result<SslString> ToDec() const noexcept;
  Result<SslString> ToHex() const noexcept;
  Result<SslData> ToBin() const noexcept;

  int NumberOfBytes() const;
  int NumberOfBits() const;

  BIGNUM* Ptr() noexcept;

 private:
  struct Deleter {
    void operator()(BIGNUM* number) noexcept;
  };

  using BigNumImpl = std::unique_ptr<BIGNUM, Deleter>;

  explicit BigNum(BigNumImpl ptr) noexcept;

  BigNumImpl ptr_{};
};

Result<BigNum> operator+(const BigNum& lhs, const BigNum& rhs) noexcept;

Result<BigNum> operator+(Result<BigNum>&& maybe_lhs,
                         Result<BigNum>&& maybe_rhs) noexcept;

Result<BigNum> operator+(const BigNum& lhs,
                         Result<BigNum>&& maybe_rhs) noexcept;

Result<BigNum> operator+(Result<BigNum>&& maybe_lhs,
                         const BigNum& rhs) noexcept;

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

inline Result<BigNum> BigNum::Own(BIGNUM* ptr) noexcept {
  if (!ptr) {
    return BigNumErrc::NullPointerFailure;
  }
  return BigNum{BigNumImpl{ptr}};
}

inline Result<BigNum> BigNum::New(BnUlong value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_set_word(result.Ptr(), value) == 0) {
    return BigNumErrc::ExpansionFailure;
  }
  return result;
}

inline Result<BnUlong> BigNum::ToBnUlong() const noexcept {
  const auto result = BN_get_word(ptr_.get());
  if (result == std::numeric_limits<decltype(result)>::max()) {
    return BigNumErrc::TooBigForConversion;
  }
  return result;
}

inline Result<BigNum> BigNum::New(const Dec& dec) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto ptr = result.Ptr();
  if (BN_dec2bn(&ptr, dec.value) == 0) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToDec() const noexcept {
  SslString result{BN_bn2dec(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToHex() const noexcept {
  SslString result{BN_bn2hex(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslData> BigNum::ToBin() const noexcept {
  SslData result;
  result.resize(NumberOfBytes());
  if (BN_bn2bin(ptr_.get(), result.data()) < 0) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<BigNum> BigNum::Add(const BigNum& lhs,
                                  const BigNum& rhs) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_add(result.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return BigNumErrc::AdditionFailure;
  }
  return result;
}

inline const BIGNUM* BigNum::Ptr() const noexcept { return ptr_.get(); }

inline BIGNUM* BigNum::Ptr() noexcept { return ptr_.get(); }

inline Result<BigNum> BigNum::New(const SslSpan& span) noexcept {
  BIGNUM* initial_value = nullptr;
  BigNumImpl ptr{BN_bin2bn(span.data(), span.size(), initial_value)};
  if (!ptr) {
    return BigNumErrc::ConversionFailure;
  }
  return BigNum{std::move(ptr)};
}

inline int BigNum::NumberOfBytes() const { return BN_num_bytes(ptr_.get()); }

inline int BigNum::NumberOfBits() const { return BN_num_bits(ptr_.get()); }

inline Result<BigNum> BigNum::New(const Hex& hex) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto ptr = result.Ptr();
  if (BN_hex2bn(&ptr, hex.value) == 0) {
    return BigNumErrc::AllocationFailure;
  }
  return result;
}

inline Result<BigNum> operator+(const BigNum& lhs, const BigNum& rhs) noexcept {
  return BigNum::Add(lhs, rhs);
}

inline Result<BigNum> operator+(Result<BigNum>&& maybe_lhs,
                                Result<BigNum>&& maybe_rhs) noexcept {
  OUTCOME_TRY(auto&& lhs, std::move(maybe_lhs));
  OUTCOME_TRY(auto&& rhs, std::move(maybe_rhs));
  return lhs + rhs;
}

inline Result<BigNum> operator+(const BigNum& lhs,
                                Result<BigNum>&& maybe_rhs) noexcept {
  OUTCOME_TRY(auto&& rhs, std::move(maybe_rhs));
  return lhs + rhs;
}

inline Result<BigNum> operator+(Result<BigNum>&& maybe_lhs,
                                const BigNum& rhs) noexcept {
  OUTCOME_TRY(auto&& lhs, std::move(maybe_lhs));
  return lhs + rhs;
}

inline int Sum(int a, int b) { return a + b; }

}  // namespace glassy

#endif  // BIG_NUM_H
