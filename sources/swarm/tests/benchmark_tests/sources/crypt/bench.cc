#include <random>
#include <unordered_set>

[[nodiscard]] inline std::vector<int> RandN(int n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> result;
  result.reserve(n);
  for (int i = 0; i < n; ++i) {
    result.push_back(distribution(gen));
  }
  return result;
}

static void RandNBench(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    const auto result = RandN(10'000);
    if (result.size() != 10'000) {
      abort();
    }
    benchmark::DoNotOptimize(result);
  }
}
// Register the function as a benchmark
BENCHMARK(RandNBench);

[[nodiscard]] inline std::vector<int> RandN1(int n) noexcept {
  if (n <= 0) {
    return std::vector<int>{};
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution;
  std::vector<int> result(n, 0);
  for (int i = 0; i < n; ++i) {
    result[i] = distribution(gen);
  }
  return result;
}

static void RandN1Bench(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    const auto result = RandN1(10'000);
    if (result.size() != 10'000) {
      abort();
    }
    benchmark::DoNotOptimize(result);
  }
}
// Register the function as a benchmark
BENCHMARK(RandN1Bench);

[[nodiscard]] inline std::vector<int> UniqRandN1(int n) noexcept {
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

static void UniqRandN1Bench(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    const auto result = UniqRandN1(10'000);
    if (result.size() != 10'000) {
      abort();
    }
    benchmark::DoNotOptimize(result);
  }
}
// Register the function as a benchmark
BENCHMARK(UniqRandN1Bench);
