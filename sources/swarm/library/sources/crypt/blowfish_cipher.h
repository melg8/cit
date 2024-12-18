// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef BLOWFISH_CIPHER_H
#define BLOWFISH_CIPHER_H

#include <openssl/blowfish.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>

#include <common_macro.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#elif defined(__clang__) && defined(__has_warning)
#if __has_warning("-Wdeprecated-declarations")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#endif

namespace swarm {

using ConstBytesView = std::span<const uint8_t>;
using BytesView = std::span<uint8_t>;

class BlowfishCipher {
 public:
  BlowfishCipher() = default;

  explicit BlowfishCipher(BytesView key) noexcept {
    BF_set_key(&bf_key_, key.size(), key.data());
  }

  F_INLINE auto Encrypt(ConstBytesView in, BytesView out) noexcept -> void {
    assert(out.size() >= in.size());
    assert(in.size() % 8 == 0);
    for (size_t i = 0; i < in.size(); i += 8) {
      BF_ecb_encrypt(&in[i], &out[i], &bf_key_, BF_ENCRYPT);
    }
  }

  F_INLINE auto EncryptInplace(BytesView in_out) noexcept -> void {
    Encrypt(in_out, in_out);
  }

  F_INLINE auto Decrypt(ConstBytesView in, BytesView out) noexcept -> void {
    assert(out.size() >= in.size());
    assert(in.size() % 8 == 0);
    for (size_t i = 0; i < in.size(); i += 8) {
      BF_ecb_encrypt(&in[i], &out[i], &bf_key_, BF_DECRYPT);
    }
  }

  F_INLINE auto DecryptInplace(BytesView in_out) noexcept -> void {
    Decrypt(in_out, in_out);
  }

 private:
  BF_KEY bf_key_{};
};

}  // namespace swarm

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__) && defined(__has_warning)
#if __has_warning("-Wdeprecated-declarations")
#pragma clang diagnostic pop
#endif
#endif

#endif  // BLOWFISH_CIPHER_H
