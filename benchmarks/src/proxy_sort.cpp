#include <numeric>
#include <benchmark/benchmark.h>
#include <vector>
#include <string>


/**
 * 直接排序和代理排序的基准测试
 */


struct FatPlayer {
    int id;
    int score;
    float health;

    std::string name;
    std::vector<std::string> inventory;
    char padding[1024];
};


static void BM_SortObjects(benchmark::State& state) {
    int n = state.range(0);
    std::vector<FatPlayer> v(n);
    for (auto _: state) {
        std::vector<FatPlayer> copy = v;
        std::ranges::sort(copy, {}, &FatPlayer::score);
        benchmark::DoNotOptimize(copy.data());
    }
}

static void BM_SortKeys(benchmark::State& state) {
    int n = state.range(0);
    std::vector<int> v(n);
    for (auto _: state) {
        std::vector<int> copy = v;
        std::ranges::sort(copy);
        benchmark::DoNotOptimize(copy.data());
    }
}

static void BM_SortProxy(benchmark::State& state) {
    int n = state.range(0);
    std::vector<FatPlayer> v(n);
    for (auto _ : state) {
        std::vector<int> proxy(n);
        std::iota(proxy.begin(), proxy.end(), 0);

        std::ranges::sort(proxy, std::ranges::less{}, [&](int i) {
            return v[i].score;
        });
        benchmark::DoNotOptimize(proxy.data());
    }
}


BENCHMARK(BM_SortObjects)->Range(1024, 65536);
BENCHMARK(BM_SortKeys)->Range(1024, 65536);
BENCHMARK(BM_SortProxy)->Range(1024, 65536);
