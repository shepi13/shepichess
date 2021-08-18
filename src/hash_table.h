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
  HashEntry();
  HashEntry(uint16_t depth, uint16_t eval, uint16_t best_move, HashKey key);
  uint16_t depth() const;
  uint16_t eval() const;
  uint16_t bestMove() const;
private:
  friend class HashTable;
  HashKey calculateChecksum();
  HashKey data;
  HashKey key;
  HashKey checksum;
};

class HashTable {
public:
  HashTable();
  HashTable(size_t size);
  ~HashTable() = default;
  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  HashTable(HashTable&&) = default;
  HashTable& operator=(HashTable&&) = default;

  void clear();
  void resize(size_t new_size_mb);
  void stash(HashEntry value);
  HashEntry probe(HashKey key, bool& found) const;
  
  size_t size() const;
private:
  std::vector<HashEntry> table;
  size_t hash_size;
  std::mutex lock;
};

} // namespace shepichess