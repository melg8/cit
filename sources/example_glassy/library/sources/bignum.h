#ifndef BIGNUM_H
#define BIGNUM_H

#include <openssl/bn.h>
#include <outcome.hpp>

namespace glassy {

using BnUlong = BN_ULONG;
namespace outcome = OUTCOME_V2_NAMESPACE;

template <typename T>
using Result = outcome::result<T>;

class BigNum {
 public:
  static Result<BigNum> fromBnUlong(BnUlong value) noexcept;

 private:
  int v;
};

inline Result<BigNum> BigNum::fromBnUlong(BnUlong value) noexcept {
  BigNum v;
  v.v = value;
  return v;
}

inline int Sum(int a, int b) { return a + b; }

}  // namespace glassy

#endif  // BIGNUM_H
