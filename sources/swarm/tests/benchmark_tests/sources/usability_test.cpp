// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <openssl/evp.h>
#include <testing_framework.h>

#include <placeholder_function.h>

#include <openssl/blowfish.h>
#include <array>
#include <string>
#include <vector>

namespace swarm::test {

[[nodiscard]] inline std::vector<std::byte> DataForBenchmark() noexcept {
  std::vector<std::byte> data(1024);
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = static_cast<std::byte>(i % 256);
  }
  return data;
}

[[nodiscard]] inline std::vector<unsigned char> GenerateTestData(
    size_t size) noexcept {
  std::vector<unsigned char> data(size);
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = static_cast<unsigned char>(i % 256);
  }
  return data;
}

[[nodiscard]] inline std::array<unsigned char, 21> KeyForBenchmark() noexcept {
  std::array<unsigned char, 21> key;
  for (size_t i = 0; i < 20; ++i) {
    key[i] = static_cast<unsigned char>(i);
  }
  key[20] = '\0';
  return key;
}

// SCENARIO("Basic computaion") {
//   const auto test_data = DataForBenchmark();

//   BENCHMARK("hex and ascii view") {
//     std::string hex{};
//     HexAsciiViewFrom(test_data, hex);
//     return hex;
//   };

// const auto data = GenerateTestData(1'000'000);
// const auto key = KeyForBenchmark();
// std::vector<unsigned char> encrypted(data.size());

// BENCHMARK("Blowfish encryption raw of 1MB data") {
//   BF_KEY bf_key;
//   BF_set_key(&bf_key, key.size(), key.data());
//   for (size_t i = 0; i < data.size(); i += 8) {
//     BF_ecb_encrypt(&data[i], &encrypted[i], &bf_key, BF_ENCRYPT);
//   }
//   return encrypted;
// };

// BENCHMARK("Blowfish encryption with context of 1MB data") {
//   EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
//   EVP_EncryptInit_ex(ctx, EVP_bf_ecb(), NULL, key.data(), NULL);
//   std::vector<unsigned char> enc(data.size());
//   int outlen = 0;
//   EVP_EncryptUpdate(ctx, enc.data(), &outlen, data.data(), data.size());
//   EVP_EncryptFinal_ex(ctx, enc.data() + outlen, &outlen);
//   EVP_CIPHER_CTX_free(ctx);
//   return enc;
// };
// }

}  // namespace swarm::test
