#include "position.h"

#include <cassert>
#include <cctype>
#include <random>
#include <sstream>
#include <utility>

#include <spdlog/spdlog.h>

#include "eval.h"

namespace shepichess {

namespace {

constexpr int kMoveStackSize = 1000;
static std::array<HashKey, 768> kZobristPieces;
static std::array<HashKey, 4> kZobristCastling;
static std::array<HashKey, 8> kZobristEnpassant;
static HashKey kZobristSideToMove;

constexpr std::array<std::pair<Piece, char>, 12> kPieceFenCodes {
  std::make_pair(Piece::WhiteKing, 'K'),
  std::make_pair(Piece::BlackKing, 'k'),
  std::make_pair(Piece::WhiteQueen, 'Q'),
  std::make_pair(Piece::BlackQueen, 'q'),
  std::make_pair(Piece::WhiteBishop, 'B'),
  std::make_pair(Piece::BlackBishop, 'b'),
  std::make_pair(Piece::WhiteKnight, 'N'),
  std::make_pair(Piece::BlackKnight, 'n'),
  std::make_pair(Piece::WhiteRook, 'R'),
  std::make_pair(Piece::BlackRook, 'r'),
  std::make_pair(Piece::WhitePawn, 'P'),
  std::make_pair(Piece::BlackPawn, 'p')};

std::vector<std::string> split(const std::string& str)
{
  std::istringstream iss {str};
  std::string token;
  std::vector<std::string> results;
  while (iss >> token) {
    results.push_back(std::move(token));
  }
  return results;
}

int to_int(const std::string& str)
{
  try {
    return stoi(str);
  } catch (std::invalid_argument) {
    SPDLOG_ERROR("Failed to parse int from {}", str);
    return 0;
  }
}

std::pair<Piece, bool> getPieceType(char piece_code)
{
  for (auto&& [piece, code] : kPieceFenCodes) {
    if (code == piece_code) return std::make_pair(piece, static_cast<int>(piece) > 8);
  }
  SPDLOG_ERROR("error: Invalid piece type: {}", piece_code);
  return std::make_pair(Piece::None, false);
}

char getPieceFenCode(Piece piece_val)
{
  for (auto&& [piece, code] : kPieceFenCodes) {
    if (piece == piece_val) return code;
  }
  return '-';
}

} // namespace

void initZobrist()
{
  std::mt19937_64 mt;
  std::uniform_int_distribution<HashKey> dist;
  for (auto&& elem : kZobristPieces) {
    elem = dist(mt);
  }
  for (auto&& elem : kZobristCastling) {
    elem = dist(mt);
  }
  for (auto&& elem : kZobristEnpassant) {
    elem = dist(mt);
  }
  kZobristSideToMove = dist(mt);
}

Position::Position(const std::string& fen)
{
  setPosition(fen);
}

void Position::setPosition(const std::string& fen)
{
  SPDLOG_INFO("Initializing position using fen: {}", fen);
  // Reset position data
  std::fill(pieces.begin(), pieces.end(), Piece::None);
  std::fill(pieces_by_type.begin(), pieces_by_type.end(), 0ULL);
  std::fill(pieces_by_color.begin(), pieces_by_color.end(), 0ULL);
  states.resize(0);
  states.reserve(kMoveStackSize);
  moves.resize(0);
  moves.reserve(kMoveStackSize);
  // Set Piece Data
  PositionState current_state {};
  auto args = split(fen);
  int row = 7, col = 7;
  for (auto&& piece_char : args[0]) {
    if (isdigit(piece_char)) {
      col -= piece_char - '0';
      continue;
    } else if (piece_char == '/') {
      row--;
      col = 7;
      continue;
    }
    if (col < 0 || row < 0) {
      SPDLOG_CRITICAL("Invalid Row/Column while parsing fen: ({},{})", row, col);
      throw std::logic_error("Invalid row/column while parsing fen");
    }
    auto [piece_type, color_idx] = getPieceType(piece_char);
    SPDLOG_DEBUG("Initializing ({},{}) with piece: {}", row, col, piece_char);
    if (piece_type != Piece::None) {
      pieces[row * 8 + col] = piece_type;
      bitboards::setbit(pieces_by_type[static_cast<int>(piece_type)], row * 8 + col);
      bitboards::setbit(pieces_by_color[color_idx], row * 8 + col);
      current_state.material[color_idx] += pieceValue(piece_type);
      current_state.zobrist ^=
        kZobristPieces[64 * static_cast<int>(piece_type) + row * 8 + col];
    }
    col--;
  }
  // Set side to move
  side_to_move = args[1] == "w" ? Color::White : Color::Black;
  current_state.zobrist ^= kZobristSideToMove * static_cast<int>(side_to_move);
  SPDLOG_DEBUG("Initialized side_to_move = {}", args[1]);
  // Set castling rights
  current_state.white_kingside_castle = args[2].find('K') != std::string::npos;
  current_state.black_kingside_castle = args[2].find('k') != std::string::npos;
  current_state.white_queenside_castle = args[2].find('Q') != std::string::npos;
  current_state.black_queenside_castle = args[2].find('q') != std::string::npos;
  current_state.zobrist ^= kZobristCastling[0] * current_state.white_kingside_castle;
  current_state.zobrist ^= kZobristCastling[1] * current_state.white_queenside_castle;
  current_state.zobrist ^= kZobristCastling[2] * current_state.black_kingside_castle;
  current_state.zobrist ^= kZobristCastling[3] * current_state.black_queenside_castle;
  SPDLOG_DEBUG("Initialized castling rights with: {}", args[2]);
  // Set enpassant and move counts
  if (args[3] != "-") {
    current_state.enpassant_square = static_cast<unsigned char>(args[3].at(0));
    current_state.zobrist ^= kZobristEnpassant[current_state.enpassant_square];
  }
  SPDLOG_DEBUG("Initialized en_passant square: {}", current_state.enpassant_square);
  current_state.move_count50 = to_int(args[4]);
  move_number = to_int(args[5]);
  SPDLOG_DEBUG(
    "Initialized move_count50, move_number: {}, {}",
    current_state.move_count50,
    move_number);
  // Save current state
  states.push_back(current_state);
  SPDLOG_INFO("Initialized position:\n{}", repr());
  assert(checkInvariants());
}

std::string Position::repr()
{
  std::ostringstream oss;
  for (int row = 7; row >= 0; row--) {
    for (int column = 7; column >= 0; column--) {
      oss << getPieceFenCode(pieces[row * 8 + column]);
    }
    oss << '\n';
  }
  PositionState& current_state = states.back();
  oss << "Side to move: ";
  oss << (side_to_move == Color::White ? "white" : "black");
  oss << "\nCastling Rights: ";
  if (current_state.white_kingside_castle) oss << 'K';
  if (current_state.white_queenside_castle) oss << 'Q';
  if (current_state.black_kingside_castle) oss << 'k';
  if (current_state.black_queenside_castle) oss << 'q';
  oss << "\nEnpassant file: " << static_cast<char>(current_state.enpassant_square);
  oss << "\nMoves until draw: " << 100 - current_state.move_count50;
  oss << "\nMove Number: " << move_number;
  oss << "\nZobrist: " << current_state.zobrist;
  return oss.str();
}

bool Position::checkInvariants()
{
  return true;
}

} // namespace shepichess