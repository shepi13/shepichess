#include "bitboard.h"

#include <sstream>

namespace shepichess {

    void bitboards::init() {
        // TODO: Implement magic bitboards.
    }

    std::string bitboards::repr(Bitboard board) {
        std::ostringstream ss;
        for(int i = 7; i >= 0; i--) {
            for(int j = 7; j >= 0; j--) {
                ss << getbit(board, i*8 + j) << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }

} // namespace shepichess