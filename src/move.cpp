#include <cstring>
void movePiece(unsigned int piecePositions[64], char move[5]) {

  unsigned int oldPosition, newPosition;

  // Transofrm board position to a number
  for (int i = 0; i < 2; i++) {

    (i == 0 ? move[0] : move[2]) == 'a'
        ? (i == 0 ? oldPosition : newPosition) = 0
    : (i == 0 ? move[0] : move[2]) == 'b'
        ? (i == 0 ? oldPosition : newPosition) = 1
    : (i == 0 ? move[0] : move[2]) == 'c'
        ? (i == 0 ? oldPosition : newPosition) = 2
    : (i == 0 ? move[0] : move[2]) == 'd'
        ? (i == 0 ? oldPosition : newPosition) = 3
    : (i == 0 ? move[0] : move[2]) == 'e'
        ? (i == 0 ? oldPosition : newPosition) = 4
    : (i == 0 ? move[0] : move[2]) == 'f'
        ? (i == 0 ? oldPosition : newPosition) = 5
    : (i == 0 ? move[0] : move[2]) == 'g'
        ? (i == 0 ? oldPosition : newPosition) = 6
    : (i == 0 ? move[0] : move[2]) == 'h'
        ? (i == 0 ? oldPosition : newPosition) = 7
        : oldPosition = 64;

    (i == 0 ? oldPosition : newPosition) +=
        ((7 - (((i == 0 ? move[1] : move[3]) - '0') - 1)) * 8);
  };

  // Castling
  if (move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8") {
    piecePositions[newPosition] = piecePositions[oldPosition];
    piecePositions[oldPosition] = 0;

    piecePositions[newPosition + (move[2] == 'g' ? -1 : 1)] =
        (move[1] == '1' ? 4 : 10);
    piecePositions[newPosition + (move[2] == 'g' ? 1 : -2)] = 0;
  }
  // En passant
  else if ((piecePositions[oldPosition] == 1 ||
            piecePositions[oldPosition] == 7) &&
           (piecePositions[oldPosition - 1] == 1 ||
            piecePositions[oldPosition - 1] == 7 ||
            piecePositions[oldPosition + 1] == 1 ||
            piecePositions[oldPosition + 1] == 7) &&
           ((oldPosition == newPosition + 7) ||
            (oldPosition == newPosition - 7) ||
            (oldPosition == newPosition + 9) ||
            (oldPosition == newPosition - 7))) {
    piecePositions[newPosition] = piecePositions[oldPosition];
    piecePositions[oldPosition] = 0;
    piecePositions[newPosition + (piecePositions[newPosition] == 1 ? 8 : -8)] =
        0;
  }
  // Promotion
  else if (strlen(move) == 5) {
    piecePositions[newPosition] =
        (move[4] == 'N'   ? (newPosition < oldPosition ? 2 : 8)
         : move[4] == 'B' ? (newPosition < oldPosition ? 3 : 9)
         : move[4] == 'R' ? (newPosition < oldPosition ? 4 : 10)
                          : (newPosition < oldPosition ? 5 : 11));
    piecePositions[oldPosition] = 0;
  }
  // Any other move
  else {
    piecePositions[newPosition] = piecePositions[oldPosition];
    piecePositions[oldPosition] = 0;
  }
}
