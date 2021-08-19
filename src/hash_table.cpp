#include "hash_table.h"

#include <algorithm>
#include <execution>

namespace shepichess {

namespace {

size_t previousPowerOfTwo(size_t val)
{
  size_t result = 1;
  while (result * 2 <= val) {
    result *= 2;
  }
  return result;
}

} // namespace

HashEntry::HashEntry(uint16_t depth, uint16_t eval, uint16_t best_move, HashKey key)
  : key(key)
{
  data = static_cast<HashKey>(depth) | static_cast<HashKey>(eval) << 16 |
    static_cast<HashKey>(best_move) << 32;
  checksum = calculateChecksum();
}

uint16_t HashEntry::depth() const
{
  return static_cast<uint16_t>(data);
}

uint16_t HashEntry::eval() const
{
  return static_cast<uint16_t>(data >> 16);
}

uint16_t HashEntry::bestMove() const
{
  return static_cast<uint16_t>(data >> 32);
}

HashKey HashEntry::calculateChecksum()
{
  return data ^ key;
}

HashTable::HashTable(size_t size)
{
  resize(size);
}

void HashTable::clear()
{
  lock.lock();
  std::fill(std::execution::par_unseq, table.begin(), table.end(), HashEntry {});
  lock.unlock();
}

void HashTable::resize(size_t new_size_mb)
{
  hash_size = previousPowerOfTwo(new_size_mb * 1024 * 1024 / sizeof(HashEntry));
  lock.lock();
  table.resize(hash_size);
  lock.unlock();
}

size_t HashTable::size() const
{
  return hash_size;
}

// stash and probe are racy, use checksum to check validity
void HashTable::stash(HashEntry value)
{
  HashKey mask = hash_size - 1;
  table[value.key & mask] = value;
}

std::optional<HashEntry> HashTable::probe(HashKey key) const
{
  HashKey mask = hash_size - 1;
  HashEntry value = table[key & mask];
  bool found = (key == value.key && value.checksum == value.calculateChecksum());
  return found ? std::optional<HashEntry> {value} : std::nullopt;
}

} // namespace shepichess