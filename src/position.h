#pragma once

#include <array>
#include <string>
#include <vector>

#include "bitboard.h"
#include "hash_table.h"
#include "move.h"

namespace shepichess {

void initZobrist();

const std::string kStartPositionFen {
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

enum class Color { White, Black };

enum class Piece {
  WhitePawn = 0,
  WhiteRook,
  WhiteKnight,
  WhiteBishop,
  WhiteQueen,
  WhiteKing,
  BlackPawn,
  BlackRook,
  BlackKnight,
  BlackBishop,
  BlackQueen,
  BlackKing,
  None
};

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
  Position(const std::string& fen = kStartPositionFen);
  ~Position() = default;
  Position(const Position&) = delete;
  Position(const Position&&) = delete;
  Position& operator=(const Position&) = delete;
  Position& operator=(Position&&) = delete;

  void setPosition(const std::string& fen = kStartPositionFen);
  [[nodiscard]] std::string repr();

  // For debugging
  [[nodiscard]] bool checkInvariants();

private:
  int move_number = 0;
  Color side_to_move = Color::White;
  std::array<Piece, 64> pieces {};
  std::array<Bitboard, 2> pieces_by_color {};
  std::array<Bitboard, 16> pieces_by_type {};
  std::vector<PositionState> states {};
  std::vector<Move> moves {};

  void resetPosition();
  void initPosition(const std::string& fen);
};

} // namespace shepichess
