#pragma once

#include <cstdint>
#include <string>

// Includes for popcount and other bit ops
#if __cplusplus >= 202002L
    #include <bits>
#elif defined(_WIN32)
    #include <intrin.h>
#endif

namespace shepichess {
    
    using Bitboard = uint64_t;
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

    namespace bitboards {

        void init();
        std::string repr(Bitboard);
        constexpr bool getbit(Bitboard, unsigned int);
        constexpr Bitboard setbit(Bitboard, unsigned int);
        constexpr Bitboard clearbit(Bitboard, unsigned int);
        constexpr Bitboard poplsb(Bitboard);
        constexpr int bitscan(Bitboard);
        constexpr unsigned int popcount(Bitboard);
        template<Direction> constexpr Bitboard shift(Bitboard);

        constexpr inline Bitboard ranks[8] = {
            0xffULL << 0,  0xffULL << 8,  0xffULL << 16, 0xffULL << 24,
            0xffULL << 32, 0xffULL << 40, 0xffULL << 48, 0xffULL << 56
        };       
        #define AFILE 0x8080'8080'8080'8080ULL
        constexpr inline Bitboard files[8] = {
            AFILE >> 0, AFILE >> 2, AFILE >> 3, AFILE >> 4,
            AFILE >> 5, AFILE >> 6, AFILE >> 7, AFILE >> 8
        };
        #undef AFILE

    } // namespace bitboards


    constexpr bool bitboards::getbit(Bitboard board, unsigned int idx)
    {
        return (1ULL << idx) & board; 
    }

    constexpr Bitboard bitboards::setbit(Bitboard board, unsigned int idx)
    {
        return (1ULL << idx) | board;
    }

    constexpr Bitboard bitboards::clearbit(Bitboard board, unsigned int idx) 
    {
        return ~(1ULL << idx) & board;
    }

    constexpr Bitboard bitboards::poplsb(Bitboard board) 
    {
        return board & (board - 1);
    }

    constexpr int bitboards::bitscan(Bitboard board) 
    {
        #if defined(__clang__) || defined(__GNUC__)
            return __builtin_ffsll(board) - 1;

        #elif defined(_WIN32)
            unsigned long idx;
            _BitScanForward64(&idx, board);
            return idx;

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

        #elif defined(_WIN32)
            return __popcount64(board);

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

} // namespace shepichess