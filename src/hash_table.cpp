#include "hash_table.h"

#include <algorithm>
#include <execution>

namespace shepichess {

namespace {

size_t previousPowerOfTwo(size_t val)
{
  return 0;
}

uint32_t calculateChecksum(HashEntry value)
{
  return 0;
}

} // namespace

HashTable::HashTable() : table(), hash_size(0), lock() {}

void HashTable::clear()
{
  lock.lock();
  std::fill(std::execution::par_unseq, table.begin(), table.end(), HashEntry {0});
  lock.unlock();
}

void HashTable::resize(size_t new_size_mb)
{
  hash_size = previousPowerOfTwo(new_size_mb / sizeof(HashEntry));
  lock.lock();
  table.resize(hash_size);
  lock.unlock();
}

// stash and probe are racy, use checksum to check validity
void HashTable::stash(HashKey key, HashEntry value)
{
  HashKey mask = hash_size - 1;
  value.checksum = calculateChecksum(value);
  table[key & mask] = value;
}

HashEntry HashTable::probe(HashKey key, bool& found) const
{
  HashKey mask = hash_size - 1;
  const HashEntry& value = table[key & mask];
  if (key == value.key && value.checksum == calculateChecksum(value)) {
    found = true;
    return value;
  }
  found = false;
  return HashEntry {};
}

} // namespace shepichess