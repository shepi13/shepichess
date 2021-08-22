#include <benchmark/benchmark.h>

#include "hash_table.h"

constexpr size_t kTestHashSize = 24;

static void BM_HashTableStash(benchmark::State& state)
{
  shepichess::HashTable tt(state.range(0));
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  for ([[maybe_unused]] auto _ : state) {
    tt.stash(entry);
  }
}

static void BM_HashTableProbe(benchmark::State& state)
{
  shepichess::HashTable tt(state.range(0));
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  tt.stash(entry);
  for ([[maybe_unused]] auto _ : state) {
    benchmark::DoNotOptimize(tt.probe(0xff));
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