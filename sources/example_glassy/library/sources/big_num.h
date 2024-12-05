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
#include <common_macro.h>

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
  auto operator()(gsl::owner<T*> ptr) const noexcept -> void;
};

template <typename T>
auto OpenSslFree::operator()(gsl::owner<T*> ptr) const noexcept -> void {
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
  static auto New() noexcept -> Result<BigNum>;
  static auto New(BnUlong value) noexcept -> Result<BigNum>;
  static auto New(const Dec& dec) noexcept -> Result<BigNum>;
  static auto New(const Hex& hex) noexcept -> Result<BigNum>;
  static auto New(const SslSpan& span) noexcept -> Result<BigNum>;

  // TODO(melg): maybe should make static BigNum Own(NotNullBigNumOwnerPtr ptr)
  // noexcept interface.
  static auto Own(BigNumOwnerPtr ptr) noexcept -> Result<BigNum>;

  static auto Add(const BigNum& lhs, const BigNum& rhs) noexcept
      -> Result<BigNum>;

  [[nodiscard]] auto Ptr() const noexcept -> const BIGNUM*;
  [[nodiscard]] auto ToBnUlong() const noexcept -> Result<BnUlong>;
  [[nodiscard]] auto ToDec() const noexcept -> Result<SslString>;
  [[nodiscard]] auto ToHex() const noexcept -> Result<SslString>;
  [[nodiscard]] auto ToBin() const noexcept -> Result<SslData>;

  [[nodiscard]] auto NumberOfBytes() const noexcept -> int;
  [[nodiscard]] auto NumberOfBits() const noexcept -> int;

  [[nodiscard]] auto Ptr() noexcept -> BIGNUM*;

 private:
  struct Deleter {
    auto operator()(BigNumOwnerPtr number) noexcept -> void;
  };

  using BigNumImpl = std::unique_ptr<BIGNUM, Deleter>;

  explicit BigNum(BigNumImpl ptr) noexcept;

  BigNumImpl ptr_{};
};

auto operator+(const BigNum& lhs, const BigNum& rhs) noexcept -> Result<BigNum>;

auto operator+(Result<BigNum>&& maybe_lhs, Result<BigNum>&& maybe_rhs) noexcept
    -> Result<BigNum>;

auto operator+(const BigNum& lhs, Result<BigNum>&& maybe_rhs) noexcept
    -> Result<BigNum>;

auto operator+(Result<BigNum>&& maybe_lhs, const BigNum& rhs) noexcept
    -> Result<BigNum>;

auto operator+=(BigNum& lhs, const BigNum& rhs) noexcept -> Result<void>;

FORCEINLINE auto Compare(const BigNum& lhs, const BigNum& rhs) noexcept -> int {
  return BN_cmp(lhs.Ptr(), rhs.Ptr());
}

FORCEINLINE auto operator<(const BigNum& lhs, const BigNum& rhs) noexcept
    -> bool {
  return Compare(lhs, rhs) < 0;
}

FORCEINLINE auto operator>(const BigNum& lhs, const BigNum& rhs) noexcept
    -> bool {
  return Compare(lhs, rhs) > 0;
}

FORCEINLINE auto operator==(const BigNum& lhs, const BigNum& rhs) noexcept
    -> bool {
  return Compare(lhs, rhs) == 0;
}

FORCEINLINE auto operator!=(const BigNum& lhs, const BigNum& rhs) noexcept
    -> bool {
  return Compare(lhs, rhs) != 0;
}

FORCEINLINE auto glassy::BigNum::Deleter::operator()(
    glassy::BigNumOwnerPtr number) noexcept -> void {
  BN_free(number);
}

FORCEINLINE BigNum::BigNum(BigNumImpl ptr) noexcept : ptr_{std::move(ptr)} {}

FORCEINLINE auto BigNum::New() noexcept -> Result<BigNum> {
  BigNumImpl ptr{BN_new()};
  if (!ptr) {
    return BigNumErrc::kAllocationFailure;
  }
  return BigNum{std::move(ptr)};
}

FORCEINLINE auto BigNum::Own(BigNumOwnerPtr ptr) noexcept -> Result<BigNum> {
  if (!ptr) {
    return BigNumErrc::kNullPointerFailure;
  }
  return BigNum{BigNumImpl{ptr}};
}

FORCEINLINE auto BigNum::New(BnUlong value) noexcept -> Result<BigNum> {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_set_word(result.Ptr(), value) == 0) {
    return BigNumErrc::kExpansionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::ToBnUlong() const noexcept -> Result<BnUlong> {
  const auto result = BN_get_word(ptr_.get());
  if (result == std::numeric_limits<decltype(result)>::max()) {
    return BigNumErrc::kTooBigForConversion;
  }
  return result;
}

FORCEINLINE auto BigNum::New(const Dec& dec) noexcept -> Result<BigNum> {
  OUTCOME_TRY(auto result, BigNum::New());
  auto* ptr = result.Ptr();
  if (BN_dec2bn(&ptr, dec.value) == 0) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::ToDec() const noexcept -> Result<SslString> {
  SslString result{BN_bn2dec(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::ToHex() const noexcept -> Result<SslString> {
  SslString result{BN_bn2hex(ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::ToBin() const noexcept -> Result<SslData> {
  SslData result;
  result.resize(NumberOfBytes());
  if (BN_bn2bin(ptr_.get(), result.data()) < 0) {
    return BigNumErrc::kConversionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::Add(const BigNum& lhs, const BigNum& rhs) noexcept
    -> Result<BigNum> {
  OUTCOME_TRY(auto result, BigNum::New());
  if (BN_add(result.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return BigNumErrc::kAdditionFailure;
  }
  return result;
}

FORCEINLINE auto BigNum::Ptr() const noexcept -> const BIGNUM* {
  return ptr_.get();
}

FORCEINLINE auto BigNum::Ptr() noexcept -> BIGNUM* { return ptr_.get(); }

FORCEINLINE auto BigNum::New(const SslSpan& span) noexcept -> Result<BigNum> {
  BIGNUM* initial_value = nullptr;
  BigNumImpl ptr{
      // TODO(melg): replace narrow with optional cast.
      BN_bin2bn(span.data(), gsl::narrow<int>(span.size()), initial_value)};
  if (!ptr) {
    return BigNumErrc::kConversionFailure;
  }
  return BigNum{std::move(ptr)};
}

FORCEINLINE auto BigNum::NumberOfBytes() const noexcept -> int {
  return BN_num_bytes(ptr_.get());
}

FORCEINLINE auto BigNum::NumberOfBits() const noexcept -> int {
  return BN_num_bits(ptr_.get());
}

FORCEINLINE auto BigNum::New(const Hex& hex) noexcept -> Result<BigNum> {
  OUTCOME_TRY(auto result, BigNum::New());
  auto* ptr = result.Ptr();
  if (BN_hex2bn(&ptr, hex.value) == 0) {
    return BigNumErrc::kAllocationFailure;
  }
  return result;
}

FORCEINLINE auto operator+(const BigNum& lhs, const BigNum& rhs) noexcept
    -> Result<BigNum> {
  return BigNum::Add(lhs, rhs);
}

FORCEINLINE auto operator+(Result<BigNum>&& maybe_lhs,
                           Result<BigNum>&& maybe_rhs) noexcept
    -> Result<BigNum> {
  OUTCOME_TRY(auto&& lhs, std::move(maybe_lhs));
  OUTCOME_TRY(auto&& rhs, std::move(maybe_rhs));
  return lhs + rhs;
}

FORCEINLINE auto operator+(const BigNum& lhs,
                           Result<BigNum>&& maybe_rhs) noexcept
    -> Result<BigNum> {
  OUTCOME_TRY(auto&& rhs, std::move(maybe_rhs));
  return lhs + rhs;
}

FORCEINLINE auto operator+(Result<BigNum>&& maybe_lhs,
                           const BigNum& rhs) noexcept -> Result<BigNum> {
  OUTCOME_TRY(auto&& lhs, std::move(maybe_lhs));
  return lhs + rhs;
}

FORCEINLINE auto operator+=(BigNum& lhs, const BigNum& rhs) noexcept
    -> Result<void> {
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

FORCEINLINE auto operator+=(BigNum& lhs,
                            const Result<BigNum>& maybe_rhs) noexcept
    -> Result<void> {
  if (maybe_rhs.has_error()) {
    return maybe_rhs.assume_error();
  }
  const auto& rhs = maybe_rhs.assume_value();
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

FORCEINLINE auto operator+=(Result<BigNum>& maybe_lhs,
                            const BigNum& rhs) noexcept -> Result<void> {
  if (maybe_lhs.has_error()) {
    return maybe_lhs.assume_error();
  }

  auto& lhs = maybe_lhs.assume_value();
  if (BN_add(lhs.Ptr(), lhs.Ptr(), rhs.Ptr()) == 0) {
    return outcome::failure(BigNumErrc::kAdditionFailure);
  }
  return outcome::success();
}

FORCEINLINE auto operator+=(Result<BigNum>& maybe_lhs,
                            const Result<BigNum>& maybe_rhs) noexcept
    -> Result<void> {
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

FORCEINLINE auto Sum(int lhs, int rhs) -> int { return lhs + rhs; }

}  // namespace glassy

#endif  // BIG_NUM_H
