#include "bitboard.h"

#include <catch2/catch_test_macros.hpp>

#include "uci_application.h"

using shepichess::Bitboard, shepichess::Direction;

TEST_CASE("bitboards::getbit", "[bitboard]")
{
  using shepichess::bitboards::getbit;
  REQUIRE(getbit(10, 0) == 0);
  REQUIRE(getbit(10, 1) == 1);
  REQUIRE(getbit(10, 2) == 0);
  REQUIRE(getbit(10, 3) == 1);
  REQUIRE(getbit(0x8000'0000'0000'0000ULL, 63) == 1);
  REQUIRE(getbit(0x7000'0000'0000'0000ULL, 63) == 0);
}

TEST_CASE("bitboards::setbit", "[bitboard]")
{
  using shepichess::bitboards::setbit;
  REQUIRE(setbit(10, 2) == 14);
  REQUIRE(setbit(20, 3) == 28);
  REQUIRE(setbit(30, 3) == 30);
  REQUIRE(setbit(50, 0) == 51);
  REQUIRE(setbit(0, 63) == 0x8000'0000'0000'0000ULL);
}

TEST_CASE("bitboards::clearbit", "[bitboard]")
{
  using shepichess::bitboards::clearbit;
  REQUIRE(clearbit(10, 1) == 8);
  REQUIRE(clearbit(20, 4) == 4);
  REQUIRE(clearbit(30, 3) == 22);
  REQUIRE(clearbit(0xf000'0000'0000'0000ULL, 63) == 0x7000'0000'0000'0000ULL);
}

TEST_CASE("bitboards::poplsb", "[bitboard]")
{
  using shepichess::bitboards::poplsb;
  REQUIRE(poplsb(10) == 8);
  REQUIRE(poplsb(20) == 16);
  REQUIRE(poplsb(40) == 32);
  REQUIRE(poplsb(0x8000'0000'0000'0000ULL) == 0);
}

TEST_CASE("bitboards::bitscan", "[bitboard]")
{
  using shepichess::bitboards::bitscan;
  REQUIRE(bitscan(10) == 1);
  REQUIRE(bitscan(20) == 2);
  REQUIRE(bitscan(30) == 1);
  REQUIRE(bitscan(40) == 3);
  REQUIRE(bitscan(0x8000'0000'0000'0000ULL) == 63);
}

TEST_CASE("bitboards::popcount", "[bitboard]")
{
  using shepichess::bitboards::popcount;
  REQUIRE(popcount(10) == 2);
  REQUIRE(popcount(20) == 2);
  REQUIRE(popcount(30) == 4);
  REQUIRE(popcount(50) == 3);
  REQUIRE(popcount(0xffff'ffff'ffff'ffffULL) == 64);
}

// Test that shifting a fromSquare in the center works as expected
TEST_CASE("bitboards::shift center", "[bitboard]")
{
  using shepichess::bitboards::fromSquare;
  using shepichess::bitboards::shift;

  Bitboard e4 = fromSquare(27), d4 = fromSquare(28);
  Bitboard e5 = fromSquare(35), d5 = fromSquare(36);
  REQUIRE(shift<Direction::East>(d4) == e4);
  REQUIRE(shift<Direction::West>(e4) == d4);
  REQUIRE(shift<Direction::North>(e4) == e5);
  REQUIRE(shift<Direction::South>(e5) == e4);
  REQUIRE(shift<Direction::NorthEast>(d4) == e5);
  REQUIRE(shift<Direction::SouthEast>(d5) == e4);
  REQUIRE(shift<Direction::NorthWest>(e4) == d5);
  REQUIRE(shift<Direction::SouthWest>(e5) == d4);
}

// Test that shifting doesn't wrap around across edges
TEST_CASE("bitboards::shift edge", "[bitboard]")
{
  using shepichess::bitboards::fromSquare;
  using shepichess::bitboards::shift;

  Bitboard a4 = fromSquare(31), h4 = fromSquare(24);
  Bitboard e1 = fromSquare(3), e8 = fromSquare(59);
  // East
  REQUIRE(shift<Direction::NorthEast>(h4) == 0);
  REQUIRE(shift<Direction::East>(h4) == 0);
  REQUIRE(shift<Direction::SouthEast>(h4) == 0);
  // West
  REQUIRE(shift<Direction::NorthWest>(a4) == 0);
  REQUIRE(shift<Direction::West>(a4) == 0);
  REQUIRE(shift<Direction::SouthWest>(a4) == 0);
  // North
  REQUIRE(shift<Direction::NorthWest>(e8) == 0);
  REQUIRE(shift<Direction::North>(e8) == 0);
  REQUIRE(shift<Direction::NorthEast>(e8) == 0);
  // South
  REQUIRE(shift<Direction::SouthWest>(e1) == 0);
  REQUIRE(shift<Direction::South>(e1) == 0);
  REQUIRE(shift<Direction::SouthEast>(e1) == 0);
}

TEST_CASE("attack_maps::knightAttacks", "[bitboard, attack_maps]")
{
  using shepichess::attack_maps::knightAttacks;
  shepichess::UCIApp::init();
  REQUIRE(knightAttacks(27) == 0x00'00'14'22'00'22'14'00); // e4
  REQUIRE(knightAttacks(11) == 0x00'00'00'00'14'22'00'22); // e2
  REQUIRE(knightAttacks(51) == 0x22'00'22'14'00'00'00'00); // e7
  REQUIRE(knightAttacks(31) == 0x00'00'40'20'00'20'40'00); // a4
  REQUIRE(knightAttacks(24) == 0x00'00'02'04'00'04'02'00); // h4
}

TEST_CASE("attack_maps::kingAttacks", "[bitboard, attack_maps]")
{
  using shepichess::attack_maps::kingAttacks;
  shepichess::UCIApp::init();
  REQUIRE(kingAttacks(27) == 0x00'00'00'1c'14'1c'00'00);
  REQUIRE(kingAttacks(3) == 0x00'00'00'00'00'00'1c'14);
  REQUIRE(kingAttacks(59) == 0x14'1c'00'00'00'00'00'00);
  REQUIRE(kingAttacks(31) == 0x00'00'00'c0'40'c0'00'00);
  REQUIRE(kingAttacks(24) == 0x00'00'00'03'02'03'00'00);
}

TEST_CASE("attack_maps::bishopAttacks", "[bitboard, attack_maps]")
{
  using shepichess::attack_maps::bishopAttacks;
  shepichess::UCIApp::init();
  Bitboard blockers = 0xff'ff'ff'00'00'00'ff'ff;
  REQUIRE(bishopAttacks(27, 0) == 0x80'41'22'14'00'14'22'41);
  REQUIRE(bishopAttacks(27, blockers) == 0x00'00'22'14'00'14'22'00);
}

TEST_CASE("attack_maps::rookAttacks", "[bitboard, attack_maps]")
{
  using shepichess::attack_maps::rookAttacks;
  shepichess::UCIApp::init();
  Bitboard blockers = 0xff'ff'c3'c3'c3'c3'ff'ff;
  REQUIRE(rookAttacks(27, 0) == 0x08'08'08'08'f7'08'08'08);
  REQUIRE(rookAttacks(27, blockers) == 0x00'08'08'08'76'08'08'00);
}

TEST_CASE("attack_maps::queenAttacks", "[bitboard, attack_maps]")
{
  using shepichess::attack_maps::queenAttacks;
  shepichess::UCIApp::init();
  Bitboard blockers = 0xff'ff'c3'c3'c3'c3'ff'ff;
  REQUIRE(queenAttacks(27, 0) == 0x88'49'2a'1c'f7'1c'2a'49);
  REQUIRE(queenAttacks(27, blockers) == 0x00'48'2a'1c'76'1c'2a'00);
}