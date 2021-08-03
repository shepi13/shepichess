#pragma once

#include <cstdint>
#include <string>

#if __cplusplus >= 202002L
#include <bits>
#elif defined(_WIN32)
#include <intrin.h>
#endif

namespace shepichess {
    
using Bitboard = std::uint64_t;

enum class Direction {
  North,
  East,
  South,
  West,
  NorthEast,
  SouthEast,
  SouthWest,
  NorthWest
};

// File bitboards
constexpr Bitboard kFileA = 0x8080'8080'8080'8080ULL;
constexpr Bitboard kFileB = kFileA >> 1;
constexpr Bitboard kFileG = kFileA >> 6;
constexpr Bitboard kFileH = kFileA >> 7;
constexpr Bitboard kFileAB = kFileA | kFileB;
constexpr Bitboard kFileGH = kFileG | kFileH;
// Rank bitboards
constexpr Bitboard kRank1 = 0xffULL;
constexpr Bitboard kRank2 = kRank1 << 8;
constexpr Bitboard kRank7 = kRank1 << 48;
constexpr Bitboard kRank8 = kRank1 << 56;

namespace bitboards {

void init();
std::string repr(Bitboard);

constexpr Bitboard square(unsigned int);
constexpr bool getbit(Bitboard, unsigned int);
constexpr Bitboard setbit(Bitboard, unsigned int);
constexpr Bitboard clearbit(Bitboard, unsigned int);
constexpr Bitboard poplsb(Bitboard);
constexpr int bitscan(Bitboard);
constexpr unsigned int popcount(Bitboard);
template<Direction> constexpr Bitboard shift(Bitboard);

} // namespace shepichess::bitboards


// Implementations for template and inline functions

constexpr Bitboard bitboards::square(unsigned int idx)
{
  return 1ULL << idx;
}

constexpr bool bitboards::getbit(Bitboard board, unsigned int idx)
{
  return square(idx) & board; 
}

constexpr Bitboard bitboards::setbit(Bitboard board, unsigned int idx)
{
  return square(idx) | board;
}

constexpr Bitboard bitboards::clearbit(Bitboard board, unsigned int idx) 
{
  return ~square(idx) & board;
}

constexpr Bitboard bitboards::poplsb(Bitboard board) 
{
  return board & (board - 1);
}

constexpr int bitboards::bitscan(Bitboard board) 
{
#if defined(__clang__) || defined(__GNUC__)
  return __builtin_ffsll(board) - 1;

#elif defined(_WIN64)
  #pragma intrinsic(_BitScanForward64)
  unsigned long idx;
  if(_BitScanForward64(&idx, board)) {
    return idx;
  }
  return -1;

#elif defined(_WIN32)
#pragma intrinsic(_BitScanForward)
  unsigned long lower, upper;
  if(_BitScanForward(&lower, static_cast<unsigned long>(board))) {
    return lower;
  }
  return _BitScanForward(&upper, static_cast<unsigned long>(board >> 32)) ? upper : -1;

#else   
// Todo: Use DeBruijin bitscan to support more compilers
#error "Forward BitScan not implemented, use a supported compiler"
#endif
}

constexpr unsigned int bitboards::popcount(Bitboard board)
{
#if __cplusplus > 202002L
  return std::popcount(board);

#elif defined(__clang__) || defined(__GNUC__)
  return __builtin_popcountll(board);

#elif defined(_WIN64)
  return __popcount64(board);

#elif defined(_WIN32)
  return __popcount(static_cast<unsigned long>(board) + 
         __popcount(static_cast<unsigned long>(board >> 32));

#elif defined(__INTEL_COMPILER)
  return static_cast<int>(_mm_popcnt_u64(board));

#else
  // Perform Bit Hackery - TODO: Explain this in documentation somewhere
  const int mask2 = 0x3333'3333'3333'3333ULL;
  board = board - ((board >> 1) & 0x5555'5555'5555'5555ULL;
  board = (board & mask2) + ((board >> 2) & mask2);
  board = board + ((board >> 4) & 0x0f0f'0f0f'0f0f'0f0fULL)
  return static_cast<unsigned int>((board * 0x0101'0101'0101'0101ULL) >> 56);
#endif
}

template<Direction dir>
constexpr Bitboard bitboards::shift(Bitboard board) 
{
  if constexpr(dir == Direction::North) return (board << 8); 
  if constexpr(dir == Direction::South) return (board >> 8);
  if constexpr(dir == Direction::East) return (board >> 1) & ~kFileA;
  if constexpr(dir == Direction::West) return (board << 1) & ~kFileH;
  if constexpr(dir == Direction::NorthEast) return (board << 7) & ~kFileA;
  if constexpr(dir == Direction::SouthEast) return (board >> 9) & ~kFileA;
  if constexpr(dir == Direction::NorthWest) return (board << 9) & ~kFileH;
  if constexpr(dir == Direction::SouthWest) return (board >> 7) & ~kFileH;
}

} // namespace shepichess
