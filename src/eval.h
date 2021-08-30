#pragma once

#include "position.h"

namespace shepichess {

constexpr int pieceValue(Piece piece)
{
  switch (piece) {
  case Piece::WhiteQueen:
  case Piece::BlackQueen:
    return 9;
  case Piece::WhiteRook:
  case Piece::BlackRook:
    return 5;
  case Piece::WhiteBishop:
  case Piece::WhiteKnight:
  case Piece::BlackBishop:
  case Piece::BlackKnight:
    return 3;
  case Piece::WhitePawn:
  case Piece::BlackPawn:
    return 1;
  default:
    return 0;
  }
}

} // namespace shepichess