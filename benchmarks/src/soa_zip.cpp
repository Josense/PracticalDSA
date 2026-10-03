#include <benchmark/benchmark.h>
#include <vector>
#include <string>
#include <ranges>
#include <numeric>


/**
 * 比较 Aos、SoA、SoA with zip 的性能
 */


struct Player {
    int id;
    int score;
    char payload[1024]; // Simulate heavy object
};

// 1. Array of Structs (AoS)
static void BM_AoS(benchmark::State& state) {
    int n = state.range(0);
    std::vector<Player> people(n);

    // Initialize with some data
    for (int i = 0; i < n; ++i) {
        people[i].id = i;
        people[i].score = i * 2;
    }

    long long sum = 0;
    for (auto _ : state) {
        for (const auto& p : people) {
            sum += p.id + p.score;
        }
    }
    benchmark::DoNotOptimize(sum);
}

// 2. Structure of Arrays (Manual Loop)
static void BM_SoA_Loop(benchmark::State& state) {
    int n = state.range(0);
    std::vector<int> ids(n);
    std::vector<int> scores(n);

    // Initialize with some data
    for (int i = 0; i < n; ++i) {
        ids[i] = i;
        scores[i] = i * 2;
    }

    long long sum = 0;
    for (auto _ : state) {
        for (size_t i = 0; i < n; ++i) {
            sum += ids[i] + scores[i];
        }
    }
    benchmark::DoNotOptimize(sum);
}

// 3. Structure of Arrays (Zip View)
static void BM_SoA_Zip(benchmark::State& state) {
    int n = state.range(0);
    std::vector<int> ids(n);
    std::vector<int> scores(n);

    /** 这里的代码用来提示编译器，zip 做的数组长度最短距离检查是不必要的 */
    // Our arrays cannot have different sizes
    if (ids.size() != scores.size()) {
        std::unreachable(); // C++23
    }

    // Initialize with some data
    for (int i = 0; i < n; ++i) {
        ids[i] = i;
        scores[i] = i * 2;
    }

    long long sum = 0;
    for (auto _ : state) {
        for (const auto [id, score] : std::views::zip(ids, scores)) {
            sum += id + score;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK(BM_AoS)->Range(1024, 65536);
BENCHMARK(BM_SoA_Loop)->Range(1024, 65536);
BENCHMARK(BM_SoA_Zip)->Range(1024, 65536);
