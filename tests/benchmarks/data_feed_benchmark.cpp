#include <benchmark/benchmark.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

void create_vector(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<double> v(state.range(0));
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(create_vector)->Range(2,2000);

BENCHMARK_MAIN();
