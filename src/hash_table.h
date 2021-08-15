#pragma once

#include <array>
#include <cstdint>
#include <mutex>
#include <vector>

#include "move.h"

namespace shepichess {

using HashKey = uint64_t;

struct HashEntry {
  int depth;
  int eval;
  Move best_move;
  HashKey key;
  uint32_t checksum = 0;
};

class HashTable {
  public:
  HashTable();
  ~HashTable() = default;
  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  HashTable(HashTable&&) = default;
  HashTable& operator=(HashTable&&) = default;

  void clear();
  void resize(size_t new_size_mb);
  void stash(HashKey key, HashEntry value);
  HashEntry probe(HashKey key, bool& found) const;

  private:
  std::vector<HashEntry> table;
  size_t hash_size;
  std::mutex lock;
};

} // namespace shepichess