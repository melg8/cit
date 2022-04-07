#ifndef BIGNUM_H
#define BIGNUM_H

#include <openssl/bn.h>
#include <openssl/engine.h>
#include <outcome.hpp>

#include <limits>
#include <memory>
#include <system_error>
#include <vector>

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
  ConversionFailure = 4
};

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

template <typename T, typename F>
auto operator>>=(T&& result, F func) -> decltype(func(result.value())) {
  using ResultType = decltype(func(result.value()));
  if (!result.has_value()) {
    return ResultType{result.error()};
  }
  return func(result.value());
}

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
    case BigNumErrc::ConversionFailure:
      return "failed to convert value";
  }
  return "unknown";
}

const BigNumErrorCategory big_num_error_category{};
}  // namespace

inline std::error_code make_error_code(glassy::BigNumErrc e) {
  return std::error_code{static_cast<int>(e), big_num_error_category};
}

struct OpenSslFree {
  template <typename T>
  void operator()(T* ptr) noexcept {
    OPENSSL_free(ptr);
  }  // namespace glassy
};

using SslString = std::unique_ptr<char, OpenSslFree>;
using SslData = std::vector<unsigned char>;  // TODO(melg): replace with openssl
                                             // based allocation mechanism.

class BigNum {
 public:
  static Result<BigNum> New() noexcept;
  static Result<BigNum> FromBnUlong(BnUlong value) noexcept;
  static Result<BnUlong> ToBnUlong(const BigNum& value) noexcept;

  static Result<BigNum> FromDec(const char* value) noexcept;
  static Result<SslString> ToDec(const BigNum& value) noexcept;

  static Result<BigNum> FromHex(const char* value) noexcept;
  static Result<SslString> ToHex(const BigNum& value) noexcept;

  static Result<SslData> ToBin(const BigNum& value) noexcept;
  static Result<BigNum> FromBin(const SslData& value) noexcept;

  int NumberOfBytes() const;

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

inline Result<BigNum> BigNum::FromDec(const char* value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto ptr = result.ptr_.get();
  if (BN_dec2bn(&ptr, value) == 0) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToDec(const BigNum& value) noexcept {
  SslString result{BN_bn2dec(value.ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslString> BigNum::ToHex(const BigNum& value) noexcept {
  SslString result{BN_bn2hex(value.ptr_.get())};
  if (!result.get()) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<SslData> BigNum::ToBin(const BigNum& value) noexcept {
  SslData result;
  result.resize(value.NumberOfBytes());
  if (BN_bn2bin(value.ptr_.get(), result.data()) < 0) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<BigNum> BigNum::FromBin(const SslData& value) noexcept {
  BIGNUM* initial_value = nullptr;
  BigNumImpl ptr{BN_bin2bn(value.data(), value.size(), initial_value)};
  if (!ptr) {
    return BigNumErrc::ConversionFailure;
  }
  return BigNum{std::move(ptr)};
}

inline int BigNum::NumberOfBytes() const { return BN_num_bytes(ptr_.get()); }

inline Result<BigNum> BigNum::FromHex(const char* value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());
  auto ptr = result.ptr_.get();
  if (BN_hex2bn(&ptr, value) == 0) {
    return BigNumErrc::AllocationFailure;
  }
  return result;
}

inline int Sum(int a, int b) { return a + b; }

}  // namespace glassy

#endif  // BIGNUM_H
