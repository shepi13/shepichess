#include "position.h"

#include <cassert>
#include <cctype>
#include <random>
#include <sstream>
#include <utility>

#include "eval.h"
#include "logging.h"

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

std::pair<Piece, bool> getPieceType(char piece_code)
{
  for (auto&& [piece, code] : kPieceFenCodes) {
    if (code == piece_code) return std::make_pair(piece, static_cast<int>(piece) > 5);
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
  // Tokenize fen
  auto args = split(fen);
  if (args.size() < 6) {
    SPDLOG_CRITICAL("Invalid fen: {}, not all 6 fields present", fen);
    throw std::runtime_error("Invalid fen");
  }
  // Set Piece Data
  int row = 7, col = 7;
  PositionState current_state {};
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
      Bitboard& current_type = pieces_by_type[static_cast<int>(piece_type)];
      Bitboard& current_color = pieces_by_color[color_idx];
      current_type = bitboards::setbit(current_type, row * 8 + col);
      current_color = bitboards::setbit(current_color, row * 8 + col);
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
  current_state.castling_rights[0] = args[2].find('K') != std::string::npos;
  current_state.castling_rights[1] = args[2].find('Q') != std::string::npos;
  current_state.castling_rights[2] = args[2].find('k') != std::string::npos;
  current_state.castling_rights[3] = args[2].find('q') != std::string::npos;
  for (int i = 0; i < 4; i++) {
    current_state.zobrist ^= kZobristCastling[i] * current_state.castling_rights[i];
  }
  SPDLOG_DEBUG("Initialized castling rights with: {}", args[2]);
  // Set enpassant and move counts
  if (args[3] != "-") {
    current_state.enpassant_square = static_cast<unsigned char>(args[3].at(0));
    current_state.zobrist ^= kZobristEnpassant[current_state.enpassant_square];
  }
  SPDLOG_DEBUG("Initialized en_passant square: {}", current_state.enpassant_square);
  try {
    current_state.move_count50 = stoi(args[4]);
    move_number = stoi(args[5]);
  } catch (std::invalid_argument) {
    SPDLOG_CRITICAL("Invalid fen: {}, failed to parse move counts", fen);
    throw std::runtime_error("Invalid fen");
  }
  SPDLOG_DEBUG(
    "Initialized move_count50, move_number: {}, {}",
    current_state.move_count50,
    move_number);
  // Save current state
  states.push_back(current_state);
  SPDLOG_INFO("Initialized position:\n{}", repr());
  assert(checkInvariants());
}

std::string Position::repr() const
{
  std::ostringstream oss;
  for (int row = 7; row >= 0; row--) {
    for (int column = 7; column >= 0; column--) {
      oss << getPieceFenCode(pieces[row * 8 + column]);
    }
    oss << '\n';
  }
  oss << "Side to move: ";
  oss << (sideToMove() == Color::White ? "white" : "black");
  oss << "\nCastling Rights: ";
  if (castlingRights(CastlingType::WhiteKingside)) oss << 'K';
  if (castlingRights(CastlingType::WhiteQueenside)) oss << 'Q';
  if (castlingRights(CastlingType::BlackKingside)) oss << 'k';
  if (castlingRights(CastlingType::BlackQueenside)) oss << 'q';
  oss << "\nEnpassant file: " << enpassantFile();
  oss << "\nMoves until draw: " << 100 - moveCount50();
  oss << "\nMove Number: " << moveNumber();
  oss << "\nZobrist: " << zobrist();
  return oss.str();
}

Color Position::sideToMove() const
{
  return side_to_move;
}

int Position::moveNumber() const
{
  return move_number;
}

int Position::moveCount50() const
{
  return states.back().move_count50;
}

bool Position::castlingRights(shepichess::CastlingType type) const
{
  return states.back().castling_rights[static_cast<int>(type)];
}

char Position::enpassantFile() const
{
  return static_cast<char>(states.back().enpassant_square);
}

int Position::material(Color color) const
{
  return states.back().material[static_cast<int>(color)];
}

HashKey Position::zobrist() const
{
  return states.back().zobrist;
}

Piece Position::getPiece(int square) const
{
  return pieces[square];
}

Bitboard Position::colorBitboard(Color color) const
{
  return pieces_by_color[static_cast<int>(color)];
}

Bitboard Position::typeBitboard(Piece piece) const
{
  return pieces_by_type[static_cast<int>(piece)];
}

bool Position::checkInvariants() const
{
  return true;
}

} // namespace shepichess