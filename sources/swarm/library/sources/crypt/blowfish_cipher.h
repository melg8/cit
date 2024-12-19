// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef BLOWFISH_CIPHER_H
#define BLOWFISH_CIPHER_H

#include <openssl/evp.h>

#include <cassert>
#include <span>

#include <common_macro.h>
#include <core_types.h>

namespace swarm {

using std::span;

using ConstBytesView = span<const u8>;
using BytesView = span<u8>;

class BlowfishCipher {
 public:
  BlowfishCipher() = default;

  explicit BlowfishCipher(BytesView key) noexcept : ctx_(EVP_CIPHER_CTX_new()) {
    assert(ctx_);

    EVP_EncryptInit_ex(ctx_, EVP_bf_ecb(), nullptr, key.data(), nullptr);
  }

  ~BlowfishCipher() { EVP_CIPHER_CTX_free(ctx_); }

  F_INLINE auto Encrypt(ConstBytesView in, BytesView out) noexcept -> void {
    assert(out.size() >= in.size());
    assert(in.size() % 8 == 0);

    int outlen = 0;
    EVP_EncryptUpdate(ctx_, out.data(), &outlen, in.data(), in.size());
    EVP_EncryptFinal_ex(ctx_, &out[outlen], &outlen);
  }

  F_INLINE auto EncryptInplace(BytesView in_out) noexcept -> void {
    Encrypt(in_out, in_out);
  }

  F_INLINE auto Decrypt(ConstBytesView in, BytesView out) noexcept -> void {
    assert(out.size() >= in.size());
    assert(in.size() % 8 == 0);

    int outlen = 0;
    EVP_DecryptUpdate(ctx_, out.data(), &outlen, in.data(), in.size());
    EVP_DecryptFinal_ex(ctx_, &out[outlen], &outlen);
  }

  F_INLINE auto DecryptInplace(BytesView in_out) noexcept -> void {
    Decrypt(in_out, in_out);
  }

  EVP_CIPHER_CTX* ctx_ = nullptr;
};

}  // namespace swarm

#endif  // BLOWFISH_CIPHER_H
