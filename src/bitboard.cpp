#include "bitboard.h"

#include <array>
#include <random>
#include <sstream>

#include "logging.h"


namespace shepichess {

std::string bitboards::repr(Bitboard board) {
  std::ostringstream out;
  for(int i = 7; i >= 0; i--) {
    for(int j = 7; j >= 0; j--) {
      out << bitboards::getbit(board, i*8 + j) << " ";
    }
    out << "\n";
  }
  return out.str();
}

// Magic Bitboard Implementation
namespace {

const int kAttackMapSize = 4096;

struct MagicData {
  int shift;
  Bitboard mask, constant;
  std::array<Bitboard, kAttackMapSize> attack_map;
};

template<Direction dir>
Bitboard shiftUntilBlocker(int square, Bitboard blockers) {
      Bitboard result = 0ULL, squareBB = bitboards::fromSquare(square);
      while(squareBB && !(result & blockers)) {
        squareBB = bitboards::shift<dir>(squareBB);
        result |= squareBB;
      }
      return result;
}

Bitboard generateAttacks(int square, Bitboard blockers, bool rook) {
  Bitboard result = 0;
  if(rook) {
    result |= shiftUntilBlocker<Direction::North>(square, blockers);
    result |= shiftUntilBlocker<Direction::South>(square, blockers);
    result |= shiftUntilBlocker<Direction::East>(square, blockers);
    result |= shiftUntilBlocker<Direction::West>(square, blockers);
  }
  else {
    result |= shiftUntilBlocker<Direction::NorthEast>(square, blockers);
    result |= shiftUntilBlocker<Direction::SouthEast>(square, blockers);
    result |= shiftUntilBlocker<Direction::SouthWest>(square, blockers);
    result |= shiftUntilBlocker<Direction::NorthWest>(square, blockers);
  }
  return result;
}

Bitboard generateMask(int square, bool rook)
{
  if(rook) {
    Bitboard result = 0;
    result |= shiftUntilBlocker<Direction::North>(square, 0) & ~kRank8;
    result |= shiftUntilBlocker<Direction::South>(square, 0) & ~kRank1;
    result |= shiftUntilBlocker<Direction::East>(square, 0) & ~kFileH;
    result |= shiftUntilBlocker<Direction::West>(square, 0) & ~kFileA;
    return result;
  }
  else {
    return generateAttacks(square, 0, rook) & ~kBoardEdge;
  }
}

Bitboard generateRandomConstant(Bitboard mask)
{
  Bitboard constant;
  static std::random_device rd;
  static std::mt19937_64 mt{rd()};
  static std::uniform_int_distribution<Bitboard> dist;
  do {
    constant = dist(mt) & dist(mt) & dist(mt);
  } while(bitboards::popcount(constant * mask) & 0xff00'0000'0000'0000ULL < 6);
  return constant; 
}

Bitboard generateBlockerPermutations(int index, Bitboard mask)
{
  Bitboard result = 0ULL;
  int bitcount = bitboards::popcount(mask);
  for(int i = 0; i < bitcount; i++, mask=bitboards::poplsb(mask)) {
    int lsb = bitboards::bitscan(mask);
    if(index & (1 << i)) result |= bitboards::fromSquare(lsb);
  }
  return result;
}

MagicData generateMagic(int square, bool rook)
{
  using bitboards::repr;
  // Generate all blocker/attack combinations
  std::array<Bitboard, kAttackMapSize> attacks{0}, blockers{0};
  Bitboard mask = generateMask(square, rook);
  SPDLOG_DEBUG("Generated mask for ({}, {}):\n{}", square, rook, repr(mask));
  int shift = bitboards::popcount(mask);
  for(int i = 0; i < (1 << shift); i++) {
    blockers[i] = generateBlockerPermutations(i, mask);
    attacks[i] = generateAttacks(square, blockers[i], rook);
    SPDLOG_TRACE("Generated blockers:\n{}\nattacks:\n{}", repr(blockers[i]), repr(attacks[i]));
  }
  // Attempt to map attacks into table using magic number to hash position
  while(true) {
    Bitboard constant = generateRandomConstant(mask);
    std::array<Bitboard, kAttackMapSize> magic_attacks{0};
    bool success = true;
    for(int i = 0; i < (1 << shift); i++) {
      Bitboard hash = (blockers[i] * constant) >> (64 - shift);
      if(magic_attacks[hash] == 0) magic_attacks[hash] = attacks[i]; 
      // Hash collision, need to try a new magic number
      if(magic_attacks[hash] != attacks[i]) {
        success = false;
        break;
      }
    }
    if(success) {
      SPDLOG_DEBUG("Generated Magic Data with shift={}, constant={}", shift, constant);
      return MagicData { shift, mask, constant, magic_attacks };
    }
  }
}

Bitboard generateKingAttacks(int square)
{
  using bitboards::shift;
  Bitboard squareBB = bitboards::fromSquare(square);
  return shift<Direction::North>(squareBB) |
         shift<Direction::NorthEast>(squareBB) |
         shift<Direction::East>(squareBB) |
         shift<Direction::SouthEast>(squareBB) |
         shift<Direction::South>(squareBB) |
         shift<Direction::SouthWest>(squareBB) |
         shift<Direction::West>(squareBB) |
         shift<Direction::NorthWest>(squareBB);
}

Bitboard generateKnightAttacks(int square)
{
  using bitboards::shift;
  Bitboard squareBB = bitboards::fromSquare(square);
  return shift<Direction::NorthEast>(shift<Direction::North>(squareBB)) |
  shift<Direction::NorthEast>(shift<Direction::East>(squareBB)) |
  shift<Direction::SouthEast>(shift<Direction::East>(squareBB)) |
  shift<Direction::SouthEast>(shift<Direction::South>(squareBB)) |
  shift<Direction::SouthWest>(shift<Direction::South>(squareBB)) |
  shift<Direction::SouthWest>(shift<Direction::West>(squareBB)) |
  shift<Direction::NorthWest>(shift<Direction::West>(squareBB)) |
  shift<Direction::NorthWest>(shift<Direction::North>(squareBB));
}

} // namespace


static MagicData rookAttackMap[64];
static MagicData bishopAttackMap[64];
static Bitboard knightAttackMap[64];
static Bitboard kingAttackMap[64];

void bitboards::init()
{
  static bool initialized = false;
  if(initialized) return;

  initLogging();
  SPDLOG_INFO("Initializing Bitboards");
  for(int square = 0; square < 64; square++) {
    SPDLOG_DEBUG("Initializing for square: {}", square);
    kingAttackMap[square] = generateKingAttacks(square);
    knightAttackMap[square] = generateKnightAttacks(square);
    rookAttackMap[square] = generateMagic(square, true);
    bishopAttackMap[square] = generateMagic(square, false);
  }
  initialized = true;
  SPDLOG_INFO("Bitboards successfully initialized");
}

Bitboard attack_maps::knightAttacks(unsigned int square)
{
  return knightAttackMap[square];
}

Bitboard attack_maps::kingAttacks(unsigned int square)
{
  return kingAttackMap[square];
}

Bitboard attack_maps::bishopAttacks(unsigned int square, Bitboard blockers)
{
  const MagicData& magic = bishopAttackMap[square];
  int hash = ((magic.mask & blockers) * magic.constant) >> (64 - magic.shift);
  return magic.attack_map[hash];
}

Bitboard attack_maps::rookAttacks(unsigned int square, Bitboard blockers)
{
  const MagicData& magic = rookAttackMap[square];
  int hash = ((magic.mask & blockers) * magic.constant) >> (64 - magic.shift);
  return magic.attack_map[hash];
}

Bitboard attack_maps::queenAttacks(unsigned int square, Bitboard blockers)
{
  return rookAttacks(square, blockers) | bishopAttacks(square, blockers);
}

} // namespace shepichess