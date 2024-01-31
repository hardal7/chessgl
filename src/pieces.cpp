#include <cstdint>

int main() {
  // For bitboards
  uint64_t piecePositions[12];

  enum pieceNames {
    white,
    black,

    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
  };

  piecePositions[white] = 1;
  piecePositions[black] = 1;

  piecePositions[pawn] = 1;
  piecePositions[knight] = 1;
  piecePositions[bishop] = 1;
  piecePositions[rook] = 1;
  piecePositions[queen] = 1;
  piecePositions[king] = 1;
}
