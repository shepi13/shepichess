#include "hash_table.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("HashEntry data")
{
  shepichess::HashEntry elem {5, 100, 0x905, 0xffff};
  REQUIRE(elem.depth() == 5);
  REQUIRE(elem.eval() == 100);
  REQUIRE(elem.bestMove() == 0x905);
}

TEST_CASE("HashTable resize")
{
  constexpr size_t kTestHashSizes[] = {24, 12, 10, 5};
  shepichess::HashTable tt(1);
  for (auto&& test_hash_size : kTestHashSizes) {
    tt.resize(test_hash_size);
    // Hash size must be a power of 2, with memory smaller than kTestHashSize
    size_t hash_size = tt.size();
    REQUIRE((hash_size & (hash_size - 1)) == 0);
    REQUIRE(hash_size * sizeof(shepichess::HashEntry) <= test_hash_size * 1024 * 1024);
    REQUIRE(
      hash_size * sizeof(shepichess::HashEntry) > test_hash_size * 1024 * 1024 / 2);
  }
}

TEST_CASE("HashTable probe/stash (no collision)")
{
  shepichess::HashTable tt(1);
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  bool found;
  tt.stash(entry);
  // Search nonexistant key
  shepichess::HashEntry result = tt.probe(0xfe, found);
  REQUIRE(found == false);
  // Search valid key
  result = tt.probe(0xff, found);
  REQUIRE(found == true);
  REQUIRE(result.depth() == entry.depth());
  REQUIRE(result.eval() == entry.eval());
  REQUIRE(result.bestMove() == entry.bestMove());
}

TEST_CASE("HashTable probe/stash (with collision)")
{
  shepichess::HashTable tt(1);
  shepichess::HashEntry entry1 {5, 100, 0x60, 0xff};
  shepichess::HashEntry entry2 {8, 150, 0x90, 0xff};
  bool found;
  tt.stash(entry1);
  tt.stash(entry2);
  shepichess::HashEntry result = tt.probe(0xff, found);
  REQUIRE(found == true);
  REQUIRE(result.depth() == entry2.depth());
  REQUIRE(result.eval() == entry2.eval());
  REQUIRE(result.bestMove() == entry2.bestMove());
}

TEST_CASE("HashTable clear")
{
  shepichess::HashTable tt(1);
  shepichess::HashEntry entry {5, 100, 0x60, 0xff};
  bool found;
  tt.stash(entry);
  tt.probe(0xff, found);
  REQUIRE(found == true);
  tt.clear();
  tt.probe(0xff, found);
  REQUIRE(found == false);
}