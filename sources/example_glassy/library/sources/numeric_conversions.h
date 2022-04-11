#ifndef NUMERIC_COVERSIONS_H
#define NUMERIC_COVERSIONS_H

#include <asn_1_int.h>
#include <big_num.h>

namespace glassy {
namespace convert {

namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

static Result<BigNum> FromAsn1Int(const Asn1Int& value) noexcept;
static Result<Asn1Int> FromBigNum(const BigNum& value) noexcept;

inline Result<BigNum> FromAsn1Int(const Asn1Int& value) noexcept {
  OUTCOME_TRY(auto result, BigNum::New());

  if (ASN1_INTEGER_to_BN(value.Ptr(), result.Ptr()) == nullptr) {
    return BigNumErrc::ConversionFailure;
  }
  return result;
}

inline Result<Asn1Int> FromBigNum(const BigNum& value) noexcept {
  OUTCOME_TRY(auto result,
              Asn1Int::New());  // TODO(melg): add uninitialized variant here.

  if (BN_to_ASN1_INTEGER(value.Ptr(), result.Ptr()) == nullptr) {
    return Asn1IntErrc::ConversionFailure;
  }
  return result;
}

}  // namespace convert
}  // namespace glassy

#endif  // NUMERIC_COVERSIONS_H
