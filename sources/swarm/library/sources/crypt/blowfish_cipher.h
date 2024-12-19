// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef BLOWFISH_CIPHER_H
#define BLOWFISH_CIPHER_H

#include <cassert>
#include <span>

#include <openssl/evp.h>

#include <common_macro.h>
#include <integer_types.h>

namespace swarm {

using std::span;

using CBytesView = span<const u8>;
using BytesView = span<u8>;

class BlowfishCipher final {
  using Ctx = EVP_CIPHER_CTX;

  F_INLINE static auto EncCtx() noexcept { return EVP_CIPHER_CTX_new(); }

  F_INLINE static auto CtxFree(Ctx* ctx) noexcept -> void {
    EVP_CIPHER_CTX_free(ctx);
  }

  F_INLINE static auto KeyInit(Ctx* ctx, CBytesView key) noexcept -> void {
    MUST1 EVP_EncryptInit_ex(ctx, EVP_bf_ecb(), nullptr, key.data(), nullptr);
  }

  F_INLINE static auto Encrypt(Ctx* ctx, CBytesView in, BytesView out) noexcept
      -> void {
    int out_size = 0;
    MUST1 EVP_EncryptUpdate(ctx, out.data(), &out_size, in.data(), in.size());
    MUST1 EVP_EncryptFinal_ex(ctx, &out[out_size], &out_size);
  }

  F_INLINE static auto Decrypt(Ctx* ctx, CBytesView in, BytesView out) noexcept
      -> void {
    int out_size = 0;
    MUST1 EVP_DecryptUpdate(ctx, out.data(), &out_size, in.data(), in.size());
    MUST1 EVP_DecryptFinal_ex(ctx, &out[out_size], &out_size);
  }

 public:
  F_INLINE explicit BlowfishCipher(CBytesView key) noexcept : ctx_(EncCtx()) {
    assert(ctx_);
    assert(key.size() >= 4 && key.size() <= 56);

    KeyInit(ctx_, key);
  }

  F_INLINE ~BlowfishCipher() noexcept { BlowfishCipher::CtxFree(ctx_); }

  BlowfishCipher(const BlowfishCipher& other) noexcept = delete;
  BlowfishCipher& operator=(const BlowfishCipher& other) noexcept = delete;

  F_INLINE BlowfishCipher(BlowfishCipher&& other) noexcept : ctx_(other.ctx_) {
    other.ctx_ = nullptr;
  }

  F_INLINE BlowfishCipher& operator=(BlowfishCipher&& other) noexcept {
    if (this != &other) {
      CtxFree(ctx_);
      ctx_ = other.ctx_;
      other.ctx_ = nullptr;
    }
    return *this;
  }

  F_INLINE auto Encrypt(CBytesView in, BytesView out) noexcept -> void {
    assert(in.size() % 8 == 0);
    assert(out.size() >= in.size());

    Encrypt(ctx_, in, out);
  }

  F_INLINE auto EncryptInplace(BytesView in_out) noexcept -> void {
    Encrypt(in_out, in_out);
  }

  F_INLINE auto Decrypt(CBytesView in, BytesView out) noexcept -> void {
    assert(in.size() % 8 == 0);
    assert(out.size() >= in.size());

    Decrypt(ctx_, in, out);
  }

  F_INLINE auto DecryptInplace(BytesView in_out) noexcept -> void {
    Decrypt(in_out, in_out);
  }

 private:
  EVP_CIPHER_CTX* ctx_ = nullptr;
};

}  // namespace swarm

#endif  // BLOWFISH_CIPHER_H
