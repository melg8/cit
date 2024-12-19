// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <random>
#include <unordered_set>

namespace swarm::test {

[[nodiscard]] inline std::vector<int> UniqRandN(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::unordered_set<int> unique;
  while (unique.size() < n) {
    // rand call seems slower (O(LogN) probably) than random device.
    unique.insert(std::rand());
  }
  return std::vector<int>{unique.begin(), unique.end()};
}

[[nodiscard]] inline std::vector<int> UniqRandN1(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::unordered_set<int> unique;
  while (unique.size() < n) {
    unique.insert(distribution(gen));
  }
  return std::vector<int>{unique.begin(), unique.end()};
}

[[nodiscard]] inline std::vector<int> UniqRandN2(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> unique;
  unique.reserve(n);
  while (unique.size() < n) {
    for (auto i = unique.size(); i < n; ++i) {
      unique.push_back(distribution(gen));
    }
    std::sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
  }
  std::shuffle(unique.begin(), unique.end(), gen);
  return unique;
}

[[nodiscard]] inline std::vector<int> UniqRandN21(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> unique(n, 0);
  size_t count = 0;
  while (count < n) {
    for (auto i = count; i < n; ++i) {
      unique[i] = distribution(gen);
    }
    std::sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
    count = unique.size();
    unique.resize(n, 0);
  }
  std::shuffle(unique.begin(), unique.end(), gen);
  return unique;
}

[[nodiscard]] inline std::vector<int> UniqRandN22(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> unique(n, 0);
  auto insert_pos = unique.begin();
  while (insert_pos != unique.end()) {
    for (auto i = insert_pos; i != unique.end(); ++i) {
      *i = distribution(gen);
    }
    std::sort(unique.begin(), unique.end());
    insert_pos = std::unique(unique.begin(), unique.end());
  }
  std::shuffle(unique.begin(), unique.end(), gen);
  return unique;
}

template <typename T>
inline auto MyUnique(std::vector<T>& xs) noexcept -> size_t {
  auto n = xs.size();
  if (n == 0) {
    return 0;
  }
  size_t j = 0;
  for (size_t i = 1; i < n; ++i) {
    if (xs[j] != xs[i]) {
      ++j;
      if (j < i) {
        xs[j] = xs[i];
        for (size_t k = i + 1; k < n; ++k) {
          if (xs[j] != xs[k]) {
            ++j;
            xs[j] = xs[k];
          }
        }
        break;
      }
    }
  }
  return j + 1;
}

[[nodiscard]] inline std::vector<int> UniqRandN23(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> unique(n, 0);
  size_t pos = 0;
  while (pos < n) {
    for (auto i = pos; i < n; ++i) {
      unique[i] = distribution(gen);
    }
    std::sort(unique.begin(), unique.end());
    pos = MyUnique(unique);
  }
  std::shuffle(unique.begin(), unique.end(), gen);
  return unique;
}

[[nodiscard]] inline std::vector<int> UniqRandN3(size_t n) noexcept {
  std::set<int> unique_numbers;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;

  for (size_t i = 0; i < n; ++i) {
    int num = distribution(gen);
    while (unique_numbers.find(num) != unique_numbers.end()) {
      num = distribution(gen);
    }
    unique_numbers.insert(num);
  }

  std::vector result(unique_numbers.begin(), unique_numbers.end());
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}

[[nodiscard]] inline std::vector<int> UniqRandN4(size_t n) noexcept {
  std::set<int> unique_numbers;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;

  while (unique_numbers.size() < n) {
    const auto value = distribution(gen);
    unique_numbers.insert(value);
  }
  return std::vector<int>{unique_numbers.begin(), unique_numbers.end()};
}

class RandomSequenceOfUnique {
 private:
  unsigned int m_index = 0;
  unsigned int m_intermediateOffset = 0;

  [[nodiscard]] static inline unsigned int permuteQPR(unsigned int x) noexcept {
    static const unsigned int prime = 4294967291u;
    if (x >= prime) {
      return x;  // The 5 integers out of range are mapped to themselves.
    }
    const unsigned int residue = ((unsigned long long)x * x) % prime;
    return (x <= prime / 2) ? residue : prime - residue;
  }

 public:
  inline RandomSequenceOfUnique(unsigned int seedBase,
                                unsigned int seedOffset) noexcept {
    m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
    m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
  }

  [[nodiscard]] inline unsigned int next() noexcept {
    return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^
                      0x5bf03635);
  }
};

[[nodiscard]] inline std::vector<int> UniqRandN5(size_t n) noexcept {
  std::vector<int> result;
  result.reserve(n);
  const auto seed = static_cast<unsigned int>(time(nullptr));
  RandomSequenceOfUnique rsu(seed, seed + 1);
  for (size_t i = 0; i < n; ++i) {
    result.push_back(rsu.next());
  }

  return result;
}

[[nodiscard]] inline std::vector<int> RandN(size_t n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> result;
  result.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    result.push_back(distribution(gen));
  }
  return result;
}

SCENARIO("blowfish encryption") {
  // BENCHMARK("generate random values UniqRandN") {
  //   const auto result = UniqRandN(10'000);
  //   if (result.size() != 10'000) {
  //     abort();
  //   }
  //   return result;
  // };

  BENCHMARK("generate random values UniqRandN1") {
    const auto result = UniqRandN1(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN2") {
    const auto result = UniqRandN2(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN21") {
    const auto result = UniqRandN21(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN22") {
    const auto result = UniqRandN22(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN23") {
    const auto result = UniqRandN23(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN3") {
    const auto result = UniqRandN3(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN4") {
    const auto result = UniqRandN4(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  BENCHMARK("generate random values UniqRandN5") {
    const auto result = UniqRandN5(100'000);
    if (result.size() != 100'000) {
      abort();
    }
    return result;
  };

  // BENCHMARK("generate random values RandN") {
  //   const auto result = RandN(100'000);
  //   if (result.size() != 100'000) {
  //     abort();
  //   }
  //   return result;
  // };
}

}  // namespace swarm::test
