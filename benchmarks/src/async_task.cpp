#include <benchmark/benchmark.h>
#include <chrono>
#include <thread>
#include <future>

/**
 * 将原本主线程 10ms 的任务中的 9ms 放到子线程执行
 */

using namespace std::chrono;

void spin_for(milliseconds duration) {
    auto start = high_resolution_clock::now();
    while (high_resolution_clock::now() - start < duration) {
        benchmark::DoNotOptimize(start);
    }
}

// Terrible code
void SingleThreaded() {
    spin_for(10ms);
}

// I can fix it
void MultiThreaded() {
    auto task = std::async([](){
      spin_for(9ms);
    });
    spin_for(1ms);
    task.wait();
}

static void BM_SingleThreaded(benchmark::State& state) {
    for (auto _ : state) {
        SingleThreaded();
    }
}

static void BM_MultiThreaded(benchmark::State& state) {
    for (auto _ : state) {
        MultiThreaded();
    }
}

BENCHMARK(BM_SingleThreaded)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_MultiThreaded)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_MultiThreaded)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime();
