#pragma once

#include <array>
#include <string>
#include <vector>

#include "bitboard.h"
#include "hash_table.h"
#include "piece.h"
#include "move.h"

namespace shepichess {

void initZobrist();

const std::string kStartPositionFen {
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

enum CastlingType { WhiteKingside, WhiteQueenside, BlackKingside, BlackQueenside };

struct PositionState {
  std::array<bool, 4> castling_rights;
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
  [[nodiscard]] std::string repr() const;

  // Board state
  [[nodiscard]] Color sideToMove() const;
  [[nodiscard]] int moveNumber() const;
  [[nodiscard]] int moveCount50() const;
  [[nodiscard]] bool castlingRights(CastlingType type) const;
  [[nodiscard]] char enpassantFile() const;
  [[nodiscard]] int material(Color color) const;
  [[nodiscard]] HashKey zobrist() const;
  // Pieces/Bitboards
  [[nodiscard]] Piece getPiece(int square) const;
  [[nodiscard]] Bitboard colorBitboard(Color color) const;
  [[nodiscard]] Bitboard typeBitboard(Piece piece) const;
  // For debugging
  [[nodiscard]] bool checkInvariants() const;

private:
  int move_number = 0;
  Color side_to_move = Color::White;
  std::array<Piece, 64> pieces {};
  std::array<Bitboard, 2> pieces_by_color {};
  std::array<Bitboard, 16> pieces_by_type {};
  std::vector<PositionState> states {};
  std::vector<Move> moves {};
};

} // namespace shepichess
