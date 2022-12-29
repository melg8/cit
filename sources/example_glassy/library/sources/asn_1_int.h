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

// Possible combinations
// owned or viewed
// constant or variable
// not_null or nullable

// Functions signatures conversion.
// asn_in(const ASN1_INTEGER *x); -> AsnIn(const Asn1IntConstView &view);
// asn_in_out(ASN1_INTEGER *a); -> AsnInOut(Asn1IntView& view_modifiable);
// asn_in_consume(ASN1_INTEGER *a); -> AsnInConsume(Asn1Int && owner_move_in);
// ASN1_INTEGER *asn_out_return(); -> Result<Asn1Int> AsnOutReturn();

using Asn1IntegerOwnerPtr = gsl::owner<ASN1_INTEGER*>;
using Asn1IntegerNotNull = gsl::not_null<ASN1_INTEGER*>;
using Asn1IntegerConstNotNull = gsl::not_null<const ASN1_INTEGER*>;

struct Asn1IntDeleter {
  void operator()(Asn1IntegerOwnerPtr number) const noexcept;
};

inline void Asn1IntDeleter::operator()(
    Asn1IntegerOwnerPtr number) const noexcept {
  ASN1_INTEGER_free(number);
}

struct Asn1IntOwner {
  using Asn1IntHolder = std::unique_ptr<ASN1_INTEGER, Asn1IntDeleter>;

  explicit Asn1IntOwner(gsl::not_null<Asn1IntHolder> ptr) noexcept;

  Asn1IntegerConstNotNull Ptr() const noexcept;
  Asn1IntegerNotNull Ptr() noexcept;

 private:
  gsl::not_null<Asn1IntHolder> ptr_;
};

inline Asn1IntOwner::Asn1IntOwner(gsl::not_null<Asn1IntHolder> ptr) noexcept
    : ptr_{std::move(ptr)} {}

inline Asn1IntegerConstNotNull Asn1IntOwner::Ptr() const noexcept {
  return ptr_.get();
}

inline Asn1IntegerNotNull Asn1IntOwner::Ptr() noexcept { return ptr_.get(); }

struct Asn1IntView {
  // cppcheck-suppress noExplicitConstructor
  Asn1IntView(Asn1IntegerNotNull ptr);  // NOLINT

  // cppcheck-suppress noExplicitConstructor
  Asn1IntView(Asn1IntOwner& view);  // NOLINT

  Asn1IntegerConstNotNull Ptr() const noexcept;
  Asn1IntegerNotNull Ptr() noexcept;

 private:
  Asn1IntegerNotNull ptr_;
};

inline Asn1IntegerNotNull Asn1IntView::Ptr() noexcept { return ptr_; }

inline Asn1IntView::Asn1IntView(Asn1IntegerNotNull ptr)
    : ptr_{std::move(ptr)} {}

inline Asn1IntView::Asn1IntView(Asn1IntOwner& view) : ptr_{view.Ptr()} {}

inline Asn1IntegerConstNotNull Asn1IntView::Ptr() const noexcept {
  return ptr_;
}

struct Asn1IntConstView {
  // cppcheck-suppress noExplicitConstructor
  Asn1IntConstView(Asn1IntegerNotNull ptr)  // NOLINT
      : ptr_{std::move(ptr)} {}

  // cppcheck-suppress noExplicitConstructor
  Asn1IntConstView(Asn1IntegerConstNotNull ptr)  // NOLINT
      : ptr_{std::move(ptr)} {}

  // cppcheck-suppress noExplicitConstructor
  Asn1IntConstView(const Asn1IntView& view)  // NOLINT
      : ptr_{view.Ptr()} {}

  // cppcheck-suppress noExplicitConstructor
  Asn1IntConstView(const Asn1IntOwner& view)  // NOLINT
      : ptr_{view.Ptr()} {}

  Asn1IntegerConstNotNull Ptr() const noexcept;

 private:
  Asn1IntegerConstNotNull ptr_;
};

inline Asn1IntegerConstNotNull Asn1IntConstView::Ptr() const noexcept {
  return ptr_;
}

class Asn1Int : public Asn1IntOwner {
 public:
  using Asn1IntOwner::Asn1IntOwner;
  static Result<Asn1Int> New(Long value = 0) noexcept;
  static Result<Asn1Int> New(const Asn1IntConstView& view) noexcept;

  static Result<Asn1Int> Own(Asn1IntegerOwnerPtr ptr) noexcept;

 private:
  static Result<Asn1Int> NewUninitialized() noexcept;
};

Result<Long> ToLong(const Asn1IntConstView& view) noexcept;

int Compare(const Asn1IntConstView& lhs, const Asn1IntConstView& rhs) noexcept;

bool operator<(const Asn1IntConstView& lhs,
               const Asn1IntConstView& rhs) noexcept;

bool operator>(const Asn1IntConstView& lhs,
               const Asn1IntConstView& rhs) noexcept;

bool operator==(const Asn1IntConstView& lhs,
                const Asn1IntConstView& rhs) noexcept;

bool operator!=(const Asn1IntConstView& lhs,
                const Asn1IntConstView& rhs) noexcept;

inline Result<Asn1Int> Asn1Int::NewUninitialized() noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_new()};
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

inline Result<Asn1Int> Asn1Int::New(const Asn1IntConstView& view) noexcept {
  Asn1IntHolder ptr{ASN1_INTEGER_dup(view.Ptr())};
  if (!ptr) {
    return Asn1IntErrc::kCopyFailure;
  }
  return Asn1Int{std::move(ptr)};
}

inline Result<Asn1Int> Asn1Int::Own(Asn1IntegerOwnerPtr ptr) noexcept {
  if (!ptr) {
    return Asn1IntErrc::kNullPointerFailure;
  }
  return Asn1Int{Asn1IntHolder{ptr}};
}

inline Result<Long> ToLong(const Asn1IntConstView& view) noexcept {
  const auto result = ASN1_INTEGER_get(view.Ptr());
  if (result == -1) {
    return Asn1IntErrc::kConversionFailure;
  }
  return result;
}

inline int Compare(const Asn1IntConstView& lhs,
                   const Asn1IntConstView& rhs) noexcept {
  return ASN1_INTEGER_cmp(lhs.Ptr(), rhs.Ptr());
}

inline bool operator<(const Asn1IntConstView& lhs,
                      const Asn1IntConstView& rhs) noexcept {
  return Compare(lhs, rhs) < 0;
}

inline bool operator>(const Asn1IntConstView& lhs,
                      const Asn1IntConstView& rhs) noexcept {
  return Compare(lhs, rhs) > 0;
}

inline bool operator==(const Asn1IntConstView& lhs,
                       const Asn1IntConstView& rhs) noexcept {
  return Compare(lhs, rhs) == 0;
}

inline bool operator!=(const Asn1IntConstView& lhs,
                       const Asn1IntConstView& rhs) noexcept {
  return Compare(lhs, rhs) != 0;
}

}  // namespace glassy

#endif  // ASN_1_INT_H
