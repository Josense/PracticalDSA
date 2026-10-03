#include <benchmark/benchmark.h>
#include <vector>
#include <ranges>
#include <algorithm>

/**
 * 基础循环、迭代器、Views 之间的性能对比
 */


int UsingViews(const std::vector<int>& input) {
    int sum = 0;
    auto pipeline = input
        | std::views::filter([](int i){ return i % 2 == 0; })
        | std::views::transform([](int i){ return i * i; })
        | std::views::take(3);
    for (int i: pipeline) {
        sum += i;
    }
    return sum;
}

int UsingIterators(const std::vector<int>& input) {
    std::vector<int> temp;
    temp.reserve(input.size() / 2);
    std::copy_if(input.begin(), input.end(), std::back_inserter(temp), [](int i) { return i % 2 == 0; });

    for (auto& i: temp) i = i * i;
    int sum = 0;
    int count = 0;
    for (int i : temp) {
        if (count++ >= 3) break;
        sum += i;
    }
    return sum;
}

int UsingLoop(const std::vector<int>& input) {
    int count = 0;
    int sum = 0;
    for (int i : input) {
        if (i % 2 == 0) {
            sum += i * i;
            if (++count == 3) {
                break;
            }
        }
    }
    return sum;
}

static constexpr int N = 10000;

void BM_UsingViews(benchmark::State& state) {
    std::vector<int> l(N, 1);
    // l[0] = 2;
    // l[1] = 2;
    // l[2] = 2;
    // l[3] = 2;

    for (auto _ : state) {
        int result = UsingViews(l);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_UsingViews);

void BM_UsingIterators(benchmark::State& state) {
    std::vector<int> l(N, 1);
    // l[0] = 2;
    // l[1] = 2;
    // l[2] = 2;
    // l[3] = 2;

    for (auto _ : state) {
        int result = UsingIterators(l);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_UsingIterators);

void BM_UsingLoop(benchmark::State& state) {
    std::vector<int> l(N, 1);
    // l[0] = 2;
    // l[1] = 2;
    // l[2] = 2;
    // l[3] = 2;

    for (auto _ : state) {
        int result = UsingLoop(l);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_UsingLoop);