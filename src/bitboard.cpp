#include "bitboard.h"

#include <sstream>

namespace shepichess {

void bitboards::init() {
  // TODO: Implement magic bitboards.
}

std::string bitboards::repr(Bitboard board) {
  std::ostringstream out;
  for(int i = 7; i >= 0; i--) {
    for(int j = 7; j >= 0; j--) {
      out << bitboards::getbit(board, i*8 + j) << " ";
    }
    out << "\n";
  }
  return out.str();
}

} // namespace shepichess
