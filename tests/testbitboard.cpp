#include <catch2/catch_test_macros.hpp>

#include "bitboard.h"
using namespace shepichess::bitboards;

TEST_CASE("bitboards::getbit", "[bitboard]") {
    REQUIRE(getbit(10, 0) == 0);
    REQUIRE(getbit(10, 1) == 1);
    REQUIRE(getbit(10, 2) == 0);
    REQUIRE(getbit(10, 3) == 1);
    REQUIRE(getbit(0x8000'0000'0000'0000ULL, 63) == 1);
    REQUIRE(getbit(0x7000'0000'0000'0000ULL, 63) == 0);
}

TEST_CASE("bitboards::setbit", "[bitboard]") {
    REQUIRE(setbit(10, 2) == 14);
    REQUIRE(setbit(20, 3) == 28);
    REQUIRE(setbit(30, 3) == 30);
    REQUIRE(setbit(50, 0) == 51);
    REQUIRE(setbit(0, 63) == 0x8000'0000'0000'0000ULL);
}

TEST_CASE("bitboards::clearbit", "[bitboard]") {
    REQUIRE(clearbit(10, 1) == 8);
    REQUIRE(clearbit(20, 4) == 4);
    REQUIRE(clearbit(30, 3) == 22);
    REQUIRE(clearbit(0xf000'0000'0000'0000ULL, 63) == 0x7000'0000'0000'0000ULL);
}

TEST_CASE("bitboards::poplsb", "[bitboard]") {
    REQUIRE(poplsb(10) == 8);
    REQUIRE(poplsb(20) == 16);
    REQUIRE(poplsb(40) == 32);
    REQUIRE(poplsb(0x8000'0000'0000'0000ULL) == 0);
}

TEST_CASE("bitboards::bitscan", "[bitboard]") {
    REQUIRE(bitscan(10) == 1);
    REQUIRE(bitscan(20) == 2);
    REQUIRE(bitscan(30) == 1);
    REQUIRE(bitscan(40) == 3);
    REQUIRE(bitscan(0x8000'0000'0000'0000ULL) == 63);
}

TEST_CASE("bitboards::popcount", "[bitboard]") {
    REQUIRE(popcount(10) == 2);
    REQUIRE(popcount(20) == 2);
    REQUIRE(popcount(30) == 4);
    REQUIRE(popcount(50) == 3);
    REQUIRE(popcount(0xffff'ffff'ffff'ffffULL) == 64);
}