#include <benchmark/benchmark.h>
#include <vector>
#include <mutex>
#include <execution>
#include <algorithm>

/**
 * 多线程并发，但是关键代码需要互斥访问
 * 使用墙上时钟来衡量
 */
const std::vector<int> WORK_ITEMS(1000000, 1);

// 1. Single-Threaded (Correct)
static void BM_Seq(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        std::for_each(
          std::execution::seq,
          WORK_ITEMS.begin(), WORK_ITEMS.end(),
          [&](int) {
            counter++;
          }
        );
    }
    benchmark::DoNotOptimize(counter);
}

// 2. Multi-Threaded (Incorrect)
static void BM_Unsafe(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        std::for_each(
          std::execution::par,
          WORK_ITEMS.begin(),
          WORK_ITEMS.end(),
          [&](int) {
            // Fast, but wrong
            counter++;
          }
        );
    }
    benchmark::DoNotOptimize(counter);
}

// 3. Multi-Threaded (Correct - Mutex)
static void BM_Mutex(benchmark::State& state) {
    int counter = 0;
    std::mutex mtx; // The guard

    for (auto _ : state) {
        std::for_each(
          std::execution::par,
          WORK_ITEMS.begin(),
          WORK_ITEMS.end(),
          [&](int) {
            std::scoped_lock lock(mtx); // Acquire (Bottleneck)
            counter++;                  // Modify
          }                             // Release
        );
    }
    benchmark::DoNotOptimize(counter);
}

// 4. Multi-Threaded (Correct - Atomic)
static void BM_Atomic(benchmark::State& state) {
    std::atomic<int> counter{0};

    for (auto _ : state) {
        std::for_each(
          std::execution::par,
          WORK_ITEMS.begin(), WORK_ITEMS.end(),
          [&](int) {
            // Hardware-level synchronization
            counter++;
          }
        );
    }
}

// 5. Multi-Threaded (Correct - Reduce)
// No shared state, no locks, no atomics
// Threads work on local data in registers and only synchronize
// once at the very end to merge results.
static void BM_Reduce(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        counter = std::reduce(
          std::execution::par,
          WORK_ITEMS.begin(), WORK_ITEMS.end(),
          0
        );
    }
    benchmark::DoNotOptimize(counter);
}

#define REGISTER_BENCHMARK(func) \
  BENCHMARK(func) \
    ->Unit(benchmark::kMicrosecond) \
    ->UseRealTime()

REGISTER_BENCHMARK(BM_Seq);
REGISTER_BENCHMARK(BM_Unsafe);
REGISTER_BENCHMARK(BM_Mutex);
REGISTER_BENCHMARK(BM_Atomic);
REGISTER_BENCHMARK(BM_Reduce);
