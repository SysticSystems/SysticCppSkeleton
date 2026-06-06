#include "systic/core.hpp"
#include <benchmark/benchmark.h>

constexpr int K_FACTORIAL_INPUT = 10;
constexpr int K_SUM_FIRST_VAL   = 42;
constexpr int K_SUM_SECOND_VAL  = 73;

static void BM_ComputeFactorial(benchmark::State& state) { // NOLINT(misc-use-anonymous-namespace)
    const systic::CoreEngine engine;
    for ([[maybe_unused]] auto iter : state) { // NOLINT(altera-unroll-loops)
        int result = engine.computeFactorial(K_FACTORIAL_INPUT);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ComputeFactorial);

static void BM_ComputeSum(benchmark::State& state) { // NOLINT(misc-use-anonymous-namespace)
    for ([[maybe_unused]] auto iter : state) { // NOLINT(altera-unroll-loops)
        int result = systic::CoreEngine::computeSum(K_SUM_FIRST_VAL, K_SUM_SECOND_VAL);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ComputeSum);