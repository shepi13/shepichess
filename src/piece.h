#pragma once

namespace shepichess {

enum class Color {
  White,
  Black
};

enum class PieceType {
  Pawn,
  Rook,
  Knight,
  Bishop,
  Queen,
  King,
  None,
};

struct Piece {
  char fenCode;
  Color color;
  PieceType type;
  uint16_t materialValue;
  int hash() const
  {
    return static_cast<int>(color) * 6 + static_cast<int>(type);
  }
};

constexpr bool operator==(const Piece& piece1, const Piece& piece2)
{
  return piece1.type == piece2.type && piece1.color == piece2.color;
}

constexpr bool operator!=(const Piece& piece1, const Piece& piece2)
{
  return !(piece1 == piece2);
}

namespace pieces {

constexpr Piece WhitePawn {'P', Color::White, PieceType::Pawn, 100};
constexpr Piece BlackPawn {'p', Color::Black, PieceType::Pawn, 100};
constexpr Piece WhiteRook {'R', Color::White, PieceType::Rook, 500};
constexpr Piece BlackRook {'r', Color::Black, PieceType::Rook, 500};
constexpr Piece WhiteKnight {'N', Color::White, PieceType::Knight, 300};
constexpr Piece BlackKnight {'n', Color::Black, PieceType::Knight, 300};
constexpr Piece WhiteBishop {'B', Color::White, PieceType::Bishop, 300};
constexpr Piece BlackBishop {'b', Color::Black, PieceType::Bishop, 300};
constexpr Piece WhiteQueen {'Q', Color::White, PieceType::Queen, 900};
constexpr Piece BlackQueen {'q', Color::Black, PieceType::Queen, 900};
constexpr Piece WhiteKing {'K', Color::White, PieceType::King, 0};
constexpr Piece BlackKing {'k', Color::Black, PieceType::King, 0};
constexpr Piece None {' ', Color::Black, PieceType::None, 0};

constexpr std::array<Piece, 12> kPieceTypes {
  WhitePawn,
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
  BlackKing
};

constexpr Piece getPiece(char fenCode)
{
  for(auto&& piece : kPieceTypes) {
    if(piece.fenCode == fenCode) return piece;
  }
  return None;
}

} // namespace shepichess::pieces

} // namespace shepichess