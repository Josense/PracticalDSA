#include <benchmark/benchmark.h>
#include <future>
#include <new>

/**
 * 虚假共享和强制分散的基准测试
 */
struct BadPadding {
    std::atomic<int> a;
    std::atomic<int> b;
};

struct GoodPadding {
    alignas(std::hardware_destructive_interference_size)
      std::atomic<int> a;
    alignas(std::hardware_destructive_interference_size)
      std::atomic<int> b;
};

static void BM_FalseSharing(benchmark::State& state) {
    BadPadding stats;
    for (auto _ : state) {
        // Launch two async tasks that run continuously
        auto f1 = std::async(std::launch::async, [&](){
          for(int i=0; i<100000; ++i) stats.a++;
        });
        auto f2 = std::async(std::launch::async, [&](){
          for(int i=0; i<100000; ++i) stats.b++;
        });

        // Wait for both to finish
        f1.wait();
        f2.wait();
    }
}

static void BM_NoSharing(benchmark::State& state) {
    GoodPadding stats;
    for (auto _ : state) {
        auto f1 = std::async(std::launch::async, [&](){
          for(int i=0; i<100000; ++i) stats.a++;
        });
        auto f2 = std::async(std::launch::async, [&](){
          for(int i=0; i<100000; ++i) stats.b++;
        });

        f1.wait();
        f2.wait();
    }
}


#define REGISTER_BENCHMARK(func) \
  BENCHMARK(func) \
    ->Unit(benchmark::kMicrosecond) \
    ->UseRealTime()

REGISTER_BENCHMARK(BM_FalseSharing);
REGISTER_BENCHMARK(BM_NoSharing);