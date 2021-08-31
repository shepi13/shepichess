#include "position.h"

#include <catch2/catch_test_macros.hpp>

#include "logging.h"
#include "uci_application.h"
using namespace shepichess;

TEST_CASE("position::setPosition side to move", "[position, fen]")
{
  UCIApp::init();
  Position position {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
  REQUIRE(position.sideToMove() == shepichess::Color::White);
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
  REQUIRE(position.sideToMove() == shepichess::Color::Black);
}

TEST_CASE("position::setPosition castling", "[position, fen]")
{
  UCIApp::init();
  Position position {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
  REQUIRE(position.castlingRights(CastlingType::WhiteKingside));
  REQUIRE(position.castlingRights(CastlingType::WhiteQueenside));
  REQUIRE(position.castlingRights(CastlingType::BlackKingside));
  REQUIRE(position.castlingRights(CastlingType::BlackQueenside));
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1");
  REQUIRE(position.castlingRights(CastlingType::WhiteKingside));
  REQUIRE(position.castlingRights(CastlingType::WhiteQueenside));
  REQUIRE(!position.castlingRights(CastlingType::BlackKingside));
  REQUIRE(!position.castlingRights(CastlingType::BlackQueenside));
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kk - 0 1");
  REQUIRE(position.castlingRights(CastlingType::WhiteKingside));
  REQUIRE(position.castlingRights(CastlingType::BlackKingside));
  REQUIRE(!position.castlingRights(CastlingType::WhiteQueenside));
  REQUIRE(!position.castlingRights(CastlingType::BlackQueenside));
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
  REQUIRE(!position.castlingRights(CastlingType::WhiteKingside));
  REQUIRE(!position.castlingRights(CastlingType::WhiteQueenside));
  REQUIRE(!position.castlingRights(CastlingType::BlackKingside));
  REQUIRE(!position.castlingRights(CastlingType::BlackQueenside));
}

TEST_CASE("position::setPosition move counts", "[position, fen]")
{
  UCIApp::init();
  Position position {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
  REQUIRE(position.moveCount50() == 0);
  REQUIRE(position.moveNumber() == 1);
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 25 5");
  REQUIRE(position.moveCount50() == 25);
  REQUIRE(position.moveNumber() == 5);
}

TEST_CASE("position::setPosition enPassant file", "[position, fen]")
{
  UCIApp::init();
  Position position {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
  REQUIRE(!position.enpassantFile());
  position.setPosition("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq e3 0 1");
  REQUIRE(position.enpassantFile() == 'e');
}

TEST_CASE("position::setPosition pieces", "[position, fen]") 
{
  UCIApp::init();
  Position position;
  REQUIRE(position.checkInvariants());
  position.setPosition("7K/8/k1P5/7p/8/8/8/8 w - - 0 1");
  REQUIRE(position.checkInvariants());
  std::array<Piece, 64> expected_pieces {};
  std::fill(expected_pieces.begin(), expected_pieces.end(), Piece::None);
  expected_pieces[32] = Piece::BlackPawn;
  expected_pieces[45] = Piece::WhitePawn;
  expected_pieces[47] = Piece::BlackKing;
  expected_pieces[56] = Piece::WhiteKing;
  REQUIRE(position.colorBitboard(Color::White) == 0x01'00'20'00'00'00'00'00);
  REQUIRE(position.colorBitboard(Color::Black) == 0x00'00'80'01'00'00'00'00);
  for(int i = 0; i < 64; i++) {
    SPDLOG_TRACE("Checking piece on square {}", i);
    REQUIRE(position.getPiece(i) == expected_pieces[i]);
  }
}