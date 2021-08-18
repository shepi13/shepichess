#pragma once

#include <vector>

#include "bitboard.h"
#include "hash_table.h"
#include "move.h"

namespace shepichess {

enum class Piece {
  WhitePawn = 0,
  WhiteRook,
  WhiteKnight,
  WhiteBishop,
  WhiteQueen,
  WhiteKing,
  BlackPawn = 8,
  BlackRook,
  BlackKnight,
  BlackBishop,
  BlackQueen,
  BlackKing
};

enum class Color { White, Black };

struct PositionState {
  bool white_kingside_castle;
  bool white_queenside_castle;
  bool black_kingside_castle;
  bool black_queenside_castle;
  uint16_t move_count50;
  uint16_t enpassant_square;
  uint16_t material[2];
  HashKey zobrist;
};

class Position {
public:
  Position() = default;
  ~Position() = default;
  Position(const Position&) = delete;
  Position(const Position&&) = delete;
  Position& operator=(const Position&) = delete;
  Position& operator=(Position&&) = delete;

private:
  int move_number;
  Color side_to_move;
  Piece pieces[64];
  Bitboard pieces_by_color[2];
  Bitboard pieces_by_type[16];
  std::vector<PositionState> states;
  std::vector<Move> moves;
  // Zobrist constants
  static std::array<HashKey, 768> zobrist_pieces;
  static std::array<HashKey, 4> zobrist_castling;
  static std::array<HashKey, 8> zobrist_enpassant;
  static HashKey zobrist_side_to_move;
};

} // namespace shepichess
