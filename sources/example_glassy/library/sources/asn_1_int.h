// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef ASN_1_INT_H
#define ASN_1_INT_H

#include <memory>
#include <utility>

#include <asn_1_int_errc.h>

#include <openssl/asn1.h>
#include <gsl/gsl-lite.hpp>
#include <outcome.hpp>

namespace glassy {

using Long = long;  // NOLINT
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

using Asn1IntegerOwnerPtr = gsl::owner<ASN1_INTEGER*>;

class Asn1Int {
 public:
  static Result<Asn1Int> New(Long value = 0) noexcept;
  static Result<Asn1Int> New(const Asn1Int& other) noexcept;

  static Result<Asn1Int> Own(Asn1IntegerOwnerPtr ptr) noexcept;

  Result<Long> ToLong() const noexcept;

  const ASN1_INTEGER* Ptr() const noexcept;
  ASN1_INTEGER* Ptr() noexcept;

 private:
  struct Deleter {
    void operator()(Asn1IntegerOwnerPtr number) const noexcept;
  };

  using Asn1IntImpl = std::unique_ptr<ASN1_INTEGER, Deleter>;

  static Result<Asn1Int> NewUninitialized() noexcept;

  explicit Asn1Int(Asn1IntImpl ptr) noexcept;

  Asn1IntImpl ptr_{};
};

int Compare(const Asn1Int& lhs, const Asn1Int& rhs) noexcept;

bool operator<(const Asn1Int& lhs, const Asn1Int& rhs) noexcept;

bool operator>(const Asn1Int& lhs, const Asn1Int& rhs) noexcept;

bool operator==(const Asn1Int& lhs, const Asn1Int& rhs) noexcept;

bool operator!=(const Asn1Int& lhs, const Asn1Int& rhs) noexcept;

inline void Asn1Int::Deleter::operator()(
    Asn1IntegerOwnerPtr number) const noexcept {
  ASN1_INTEGER_free(number);
}

inline Result<Asn1Int> Asn1Int::NewUninitialized() noexcept {
  Asn1IntImpl ptr{ASN1_INTEGER_new()};
  if (!ptr) {
    return Asn1IntErrc::kAllocationFailure;
  }
  return Asn1Int{std::move(ptr)};
}

inline glassy::Result<glassy::Asn1Int> glassy::Asn1Int::New(
    Long value) noexcept {
  OUTCOME_TRY(auto result, Asn1Int::NewUninitialized());
  if (ASN1_INTEGER_set(result.Ptr(), value) == 0) {
    return Asn1IntErrc::kAllocationFailure;
  }
  return result;
}

inline Result<Asn1Int> Asn1Int::New(const Asn1Int& other) noexcept {
  Asn1IntImpl ptr{ASN1_INTEGER_dup(other.Ptr())};
  if (!ptr) {
    return Asn1IntErrc::kCopyFailure;
  }
  return Asn1Int{std::move(ptr)};
}

inline Result<Asn1Int> Asn1Int::Own(Asn1IntegerOwnerPtr ptr) noexcept {
  if (!ptr) {
    return Asn1IntErrc::kNullPointerFailure;
  }
  return Asn1Int{Asn1IntImpl{ptr}};
}

inline Result<Long> Asn1Int::ToLong() const noexcept {
  const auto result = ASN1_INTEGER_get(Ptr());
  if (result == -1) {
    return Asn1IntErrc::kConversionFailure;
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
