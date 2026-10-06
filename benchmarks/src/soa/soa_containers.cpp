#include <benchmark/benchmark.h>
#include <dsa/PlayerStorage.h>

/**
 * Lesson 6-1: SoA 自定义专属容器
 */

// 1. Original AoS
struct Player {
    int id;
    int score;
    float health;
    std::string name;
};

static void BM_AoS(benchmark::State& state) {
    int n = state.range(0);
    std::vector<Player> players(n, Player{1, 2, 3.f, "Name"});

    for (auto _ : state) {
        float sum = 0.0f;
        for (const Player& player : players) {
            sum += player.score;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// 2. PlayerStorage
static void BM_PlayerStorage(benchmark::State& state) {
    int n = state.range(0);
    PlayerStorage ps;
    for(int i=0; i<n; ++i) ps.AddPlayer(1, 2, 3.0f, "Name");

    for (auto _ : state) {
        float sum = 0.0f;
        for (const PlayerRef player : ps.GetView()) {
            sum += player.score;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// 3. Raw Manual Loop
static void BM_RawLoop(benchmark::State& state) {
    int n = state.range(0);
    std::vector<int> scores(n, 2);

    for (auto _ : state) {
        float sum = 0.0f;
        for (int i=0; i<n; ++i) {
            sum += scores[i];
        }
        benchmark::DoNotOptimize(sum);
    }
}

#define BENCHMARK_STD(func) \
  BENCHMARK(func) \
    ->RangeMultiplier(10) \
    ->Range(10 * 1000, 100000 * 1000) \
    ->Unit(benchmark::kMillisecond)

BENCHMARK_STD(BM_AoS);
BENCHMARK_STD(BM_PlayerStorage);
BENCHMARK_STD(BM_RawLoop);
