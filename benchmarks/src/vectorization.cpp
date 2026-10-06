#include <benchmark/benchmark.h>
#include <vector>
#include <random>

/**
 * 自动向量化 benchmark
 */

// Generate 1 million floats
std::vector<float> GetData() {
    std::vector<float> data(1000000);
    std::fill(data.begin(), data.end(), 1.0f);
    return data;
}

const std::vector<float> DATA = GetData();

#if defined(__GNUC__) && !defined(__clang__)
__attribute__((optimize("no-tree-vectorize")))
void ScalarAlgorithm(float* data, int count) {
    // Disable vectorization of the next loop
#if defined(__clang__)
    #pragma clang loop vectorize(disable)
#elif defined(_MSC_VER)
    #pragma loop(no_vector)
#endif
    for (size_t i = 0; i < count; ++i) {
        data[i] = std::sqrt(data[i]);
    }
}
#endif


void VectorAlgorithm(float* data, int count) {
    for (int i = 0; i < count; ++i) {
        data[i] = std::sqrt(data[i]);
    }
}

static void BM_Scalar(benchmark::State& state) {
    std::vector<float> data = DATA;
    for (auto _ : state) {
        ScalarAlgorithm(data.data(), data.size());
        benchmark::DoNotOptimize(data.data());
    }
}

static void BM_Vector(benchmark::State& state) {
    std::vector<float> data = DATA;
    for (auto _ : state) {
        VectorAlgorithm(data.data(), data.size());
        benchmark::DoNotOptimize(data.data());
    }
}

#define BENCHMARK_STD(func) \
  BENCHMARK(func) \
    ->Unit(benchmark::kMillisecond)

BENCHMARK_STD(BM_Scalar);
BENCHMARK_STD(BM_Vector);
