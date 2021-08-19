#pragma once

#include <array>
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
  std::array<uint16_t, 2> material;
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
  int move_number = 0;
  Color side_to_move = Color::White;
  std::array<Piece, 64> pieces {};
  std::array<Bitboard, 2> pieces_by_color {};
  std::array<Bitboard, 16> pieces_by_type {};
  std::vector<PositionState> states;
  std::vector<Move> moves;
  // Zobrist constants
  static std::array<HashKey, 768> zobrist_pieces;
  static std::array<HashKey, 4> zobrist_castling;
  static std::array<HashKey, 8> zobrist_enpassant;
  static HashKey zobrist_side_to_move;
};

} // namespace shepichess
