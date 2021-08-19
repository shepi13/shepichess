#include <benchmark/benchmark.h>

#include "hash_table.h"

constexpr size_t kTestHashSize = 24;

static void BM_HashTableStash(benchmark::State& state)
{
  shepichess::HashTable tt(state.range(0));
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  for (auto _ : state) {
    (void)_;
    tt.stash(entry);
  }
  benchmark::DoNotOptimize(tt);
}

static void BM_HashTableProbe(benchmark::State& state)
{
  shepichess::HashTable tt(state.range(0));
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  tt.stash(entry);
  bool found;
  for (auto _ : state) {
    (void)_;
    benchmark::DoNotOptimize(tt.probe(0xff, found));
  }
}

// Stash benchmarks
BENCHMARK(BM_HashTableStash)->Arg(kTestHashSize);
BENCHMARK(BM_HashTableStash)->Threads(2)->Arg(kTestHashSize);
BENCHMARK(BM_HashTableStash)->Threads(4)->Arg(kTestHashSize);
// Probe benchmarks
BENCHMARK(BM_HashTableProbe)->Arg(kTestHashSize);
BENCHMARK(BM_HashTableProbe)->Threads(2)->Arg(kTestHashSize);
BENCHMARK(BM_HashTableProbe)->Threads(4)->Arg(kTestHashSize);