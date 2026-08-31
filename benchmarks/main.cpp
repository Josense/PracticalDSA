#include <benchmark/benchmark.h>
#include "dsa/vector.h"

// 1. The Function
static void BM_MyVectorPush(benchmark::State& state) {
    // Setup code (Not timed)
    // We can construct objects here
    dsa::MyVector vec;

    // 2. The Measurement Loop
    for (auto _ : state) {
        // This code is timed
        vec.push_back(42);
    }
}

// 3. Register the function as a benchmark
BENCHMARK(BM_MyVectorPush);


static void BM_VectorCopy(benchmark::State& state) {
    std::vector<int> src(1000, 42);

    for (auto _ : state) {
        std::vector<int> copy{src};

        benchmark::DoNotOptimize(copy.data());
    }
}

BENCHMARK(BM_VectorCopy);