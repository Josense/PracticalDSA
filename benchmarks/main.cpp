#include <forward_list>
#include <benchmark/benchmark.h>
#include "dsa/vector.h"

/**
 * 数组追加元素操作测试
 */
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

/**
 * 数组拷贝测试
 */
static void BM_VectorCopy(benchmark::State& state) {
    std::vector<int> src(1000, 42);

    for (auto _ : state) {
        std::vector<int> copy{src};

        benchmark::DoNotOptimize(copy.data());
    }
}

BENCHMARK(BM_VectorCopy);

/**
 *数组头部插入元素测试
 */
const int TIME_COUNT = 1000;
static void BM_VectorInsertFront(benchmark::State& state) {
    for (auto _: state) {
        std::vector<int> vec;
        for (int i { 0 }; i < TIME_COUNT; i++) {
            vec.insert(vec.begin(), i);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}

BENCHMARK(BM_VectorInsertFront);

/**
 * 链表头部插入元素测试
 */
static void BM_ListInsertFront(benchmark::State& state) {
    for (auto _: state) {
        std::forward_list<int> l;
        for (int i { 0 }; i < TIME_COUNT; i++) {
            l.push_front(i);
        }
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_ListInsertFront);

/**
 * 使用随机的数据规模，并执行大O分析
 */
static void BM_ListTraverse(benchmark::State& state) {
    int64_t n = state.range(0);
    state.SetComplexityN(n);

    std::forward_list<int> l(n);
    std::fill(l.begin(), l.end(), 1);
    for (auto _: state) {
        long long sum = 0;
        for (int i: l) {
            sum += 1;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_ListTraverse)
    ->Range(8, 8 << 24)
    ->Complexity();