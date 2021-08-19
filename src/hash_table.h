#pragma once

#include <array>
#include <cstdint>
#include <mutex>
#include <vector>

#include "move.h"

namespace shepichess {

using HashKey = uint64_t;

class HashEntry {
public:
  HashEntry() = default;
  HashEntry(uint16_t depth, uint16_t eval, uint16_t best_move, HashKey key);
  [[nodiscard]] uint16_t depth() const;
  [[nodiscard]] uint16_t eval() const;
  [[nodiscard]] uint16_t bestMove() const;

private:
  friend class HashTable;
  HashKey calculateChecksum();
  HashKey data = 0;
  HashKey key = 0;
  HashKey checksum = 0;
};

class HashTable {
public:
  HashTable() = default;
  HashTable(size_t size);
  ~HashTable() = default;
  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;
  HashTable(HashTable&&) = delete;
  HashTable& operator=(HashTable&&) = delete;

  void clear();
  void resize(size_t new_size_mb);
  void stash(HashEntry value);
  HashEntry probe(HashKey key, bool& found) const;

  [[nodiscard]] size_t size() const;

private:
  size_t hash_size = 0;
  std::vector<HashEntry> table;
  std::mutex lock;
};

} // namespace shepichess