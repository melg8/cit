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

[[nodiscard]] std::vector<unsigned char> GenerateTestData(
    size_t size) noexcept {
  std::vector<unsigned char> data(size);
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = static_cast<unsigned char>(i % 256);
  }
  return data;
}

[[nodiscard]] std::array<unsigned char, 21> KeyForBenchmark() noexcept {
  std::array<unsigned char, 21> key;
  for (size_t i = 0; i < 20; ++i) {
    key[i] = static_cast<unsigned char>(i);
  }
  key[20] = '\0';
  return key;
}

SCENARIO("Basic computaion") {
  BENCHMARK("2 + 2 == 4") {
    const auto value = 2 + 2;
    if (value != 4) {
      exit(-1);
    }
    return value;
  };

  const auto test_data = DataForBenchmark();

  BENCHMARK("hex and ascii view") {
    std::string hex{};
    HexAsciiViewFrom(test_data, hex);
    if (hex.empty()) {
      exit(-1);
    }
  };

  BENCHMARK("string push back value") {
    std::string hex{};
    hex.push_back(' ');
    return hex;
  };

  BENCHMARK("string hex[0] = ' ' value") {
    std::string hex{};
    hex.reserve(47 * 5 + 20);
    hex[0] = ' ';
    hex[1] = ' ';
    hex[2] = ' ';
    return hex;
  };

  BENCHMARK("string hex += \"   \" value") {
    std::string hex{};
    hex.reserve(47 * 5 + 20);
    hex += "   ";
    return hex;
  };

  const auto data = GenerateTestData(1'000'000);
  const auto key = KeyForBenchmark();
  std::vector<unsigned char> encrypted(data.size());

  BENCHMARK("Blowfish encryption raw of 1MB data") {
    BF_KEY bf_key;
    BF_set_key(&bf_key, key.size(), key.data());
    for (size_t i = 0; i < data.size(); i += 8) {
      BF_ecb_encrypt(&data[i], &encrypted[i], &bf_key, BF_ENCRYPT);
    }
    return encrypted;
  };

  BENCHMARK("Blowfish encryption with context of 1MB data") {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_bf_ecb(), NULL, key.data(), NULL);
    std::vector<unsigned char> encrypted(data.size());
    int outlen = 0;
    EVP_EncryptUpdate(ctx, encrypted.data(), &outlen, data.data(), data.size());
    EVP_EncryptFinal_ex(ctx, encrypted.data() + outlen, &outlen);
    EVP_CIPHER_CTX_free(ctx);
    return encrypted;
  };
}

}  // namespace swarm::test
