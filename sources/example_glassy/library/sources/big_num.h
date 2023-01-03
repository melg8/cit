// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef BIG_NUM_H
#define BIG_NUM_H

#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <big_num_errc.h>

#include <openssl/bn.h>
#include <openssl/engine.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

namespace glassy {

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

struct OpenSslFree {
  template <typename T>
  void operator()(gsl::owner<T*> ptr) const noexcept;
};

template <typename T>
void OpenSslFree::operator()(gsl::owner<T*> ptr) const noexcept {
  // TODO(melg): try to implement saving of FILE/LINE from call side.
  OPENSSL_free(ptr);
}

using SslString = std::unique_ptr<char, OpenSslFree>;
using SslSpan = gsl::span<const unsigned char>;
using SslData = std::vector<unsigned char>;  // TODO(melg): replace with openssl
                                             // based allocation mechanism.
using BigNumOwnerPtr = gsl::owner<BIGNUM*>;

struct Dec {
  const char* const value{nullptr};  // NOLINT
};

struct Hex {
  const char* const value{nullptr};  // NOLINT
};

class BigNum {
 public:
  static Result<BigNum> New() noexcept;
  static Result<BigNum> New(BnUlong value) noexcept;
  static Result<BigNum> New(const Dec& dec) noexcept;
  static Result<BigNum> New(const Hex& hex) noexcept;
  static Result<BigNum> New(const SslSpan& span) noexcept;

  // TODO(melg): maybe should make static BigNum Own(NotNullBigNumOwnerPtr ptr)
  // noexcept interface.
  static Result<BigNum> Own(BigNumOwnerPtr ptr) noexcept;

  static Result<BigNum> Add(const BigNum& lhs, const BigNum& rhs) noexcept;

  [[nodiscard]] const BIGNUM* Ptr() const noexcept;
  [[nodiscard]] Result<BnUlong> ToBnUlong() const noexcept;
  [[nodiscard]] Result<SslString> ToDec() const noexcept;
  [[nodiscard]] Result<SslString> ToHex() const noexcept;
  [[nodiscard]] Result<SslData> ToBin() const noexcept;

  [[nodiscard]] int NumberOfBytes() const;
  [[nodiscard]] int NumberOfBits() const;

  BIGNUM* Ptr() noexcept;

 private:
  struct Deleter {
    void operator()(BigNumOwnerPtr number) noexcept;
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

Result<void> operator+=(BigNum& lhs, const BigNum& rhs) noexcept;

inline int Compare(const BigNum& lhs, const BigNum& rhs) noexcept {
  return BN_cmp(lhs.Ptr(), rhs.Ptr());
}

inline bool operator<(const BigNum& lhs, const BigNum& rhs) noexcept {
  return Compare(lhs, rhs) < 0;
}

inline bool operator>(const BigNum& lhs, const BigNum& rhs) noexcept {
  return Compare(lhs, rhs) > 0;
}

inline bool operator==(const BigNum& lhs, const BigNum& rhs) noexcept {
  return Compare(lhs, rhs) == 0;
}

inline bool operator!=(const BigNum& lhs, const BigNum& rhs) noexcept {
  return Compare(lhs, rhs) != 0;
}

inline void glassy::BigNum::Deleter::operator()(
    glassy::BigNumOwnerPtr number) noexcept {
  BN_free(number);
}

inline BigNum::BigNum(BigNumImpl ptr) noexcept : ptr_{std::move(ptr)} {}

inline Result<BigNum> BigNum::New() noexcept {
  BigNumImpl ptr{BN_new()};
  if (!ptr) {
    return BigNumErrc::kAllocationFailure;
  }
  return BigNum{std::move(ptr)};
}

inline Result<BigNum> BigNum::Own(BigNumOwnerPtr ptr) noexcept {
  if (!ptr) {
    return BigNumErrc::kNullPointerFailure;
  }
  return BigNum{BigNumImpl{ptr}};
}

inline Result<BigNum> BigNum::New(BnUlong value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_set_word(result.Ptr(), value) == 0) {
    return BigNumErrc::kExpansionFailure;
  }
  return result;
}

inline Result<BnUlong> BigNum::ToBnUlong() const noexcept {
  const auto result = BN_get_word(ptr_.get());
  if (result == std::numeric_limits<decltype(result)>::max()) {
    return BigNumErrc::kTooBigForConversion;
  }
  return result;
}

inline Result<BigNum> BigNum::New(const Dec& dec) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto* ptr = result.Ptr();
  if (BN_dec2bn(&ptr, dec.value) == 0) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToDec() const noexcept {
  SslString result{BN_bn2dec(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToHex() const noexcept {
  SslString result{BN_bn2hex(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

inline Result<SslData> BigNum::ToBin() const noexcept {
  SslData result;
  result.resize(NumberOfBytes());
  if (BN_bn2bin(ptr_.get(), result.data()) < 0) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

inline Result<BigNum> BigNum::Add(const BigNum& lhs,
                                  const BigNum& rhs) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_add(result.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return BigNumErrc::kAdditionFailure;
  }
  return result;
}

inline const BIGNUM* BigNum::Ptr() const noexcept { return ptr_.get(); }

inline BIGNUM* BigNum::Ptr() noexcept { return ptr_.get(); }

inline Result<BigNum> BigNum::New(const SslSpan& span) noexcept {
  BIGNUM* initial_value = nullptr;
  BigNumImpl ptr{
      // TODO(melg): replace narrow with optional cast.
      BN_bin2bn(span.data(), gsl::narrow<int>(span.size()), initial_value)};
  if (!ptr) {
    return BigNumErrc::kConversionFailure;
  }
  return BigNum{std::move(ptr)};
}

inline int BigNum::NumberOfBytes() const { return BN_num_bytes(ptr_.get()); }

inline int BigNum::NumberOfBits() const { return BN_num_bits(ptr_.get()); }

inline Result<BigNum> BigNum::New(const Hex& hex) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto* ptr = result.Ptr();
  if (BN_hex2bn(&ptr, hex.value) == 0) {
    return BigNumErrc::kAllocationFailure;
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

inline Result<void> operator+=(BigNum& lhs, const BigNum& rhs) noexcept {
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

inline Result<void> operator+=(BigNum& lhs,
                               const Result<BigNum>& maybe_rhs) noexcept {
  if (maybe_rhs.has_error()) {
    return maybe_rhs.assume_error();
  }
  const auto& rhs = maybe_rhs.assume_value();
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

inline Result<void> operator+=(Result<BigNum>& maybe_lhs,
                               const BigNum& rhs) noexcept {
  if (maybe_lhs.has_error()) {
    return maybe_lhs.assume_error();
  }

  auto& lhs = maybe_lhs.assume_value();
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

inline Result<void> operator+=(Result<BigNum>& maybe_lhs,
                               const Result<BigNum>& maybe_rhs) noexcept {
  if (maybe_lhs.has_error()) {
    return maybe_lhs.assume_error();
  }
  auto& lhs = maybe_lhs.assume_value();

  if (maybe_rhs.has_error()) {
    return maybe_rhs.assume_error();
  }
  const auto& rhs = maybe_rhs.assume_value();
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

inline int Sum(int lhs, int rhs) { return lhs + rhs; }

}  // namespace glassy

#endif  // BIG_NUM_H
