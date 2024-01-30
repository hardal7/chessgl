#include "fen.cpp"
#include <cstring>
#include <iostream>

void movePiece(unsigned long piecePositions[32], unsigned int pieceNames[32],
               char move[], bool isWhitesTurn = true) {

  int piecePosition = 0;
  (strlen(move) == 2 ? move[0] : move[1]) == 'a'   ? piecePosition += 0
  : (strlen(move) == 2 ? move[0] : move[1]) == 'b' ? piecePosition += 1
  : (strlen(move) == 2 ? move[0] : move[1]) == 'c' ? piecePosition += 2
  : (strlen(move) == 2 ? move[0] : move[1]) == 'd' ? piecePosition += 3
  : (strlen(move) == 2 ? move[0] : move[1]) == 'e' ? piecePosition += 4
  : (strlen(move) == 2 ? move[0] : move[1]) == 'f' ? piecePosition += 5
  : (strlen(move) == 2 ? move[0] : move[1]) == 'g' ? piecePosition += 6
  : (strlen(move) == 2 ? move[0] : move[1]) == 'h' ? piecePosition += 7
                                                   : piecePosition += 64;
  piecePosition +=
      (7 - (((strlen(move) == 2 ? move[1] : move[2]) - '0') - 1)) * 8;

  if (strlen(move) == 2) {
    std::cout << "Pawn moved!" << std::endl;
    for (unsigned int i = (isWhitesTurn ? 16 : 8); i < (isWhitesTurn ? 24 : 16);
         i++) {
      if (piecePositions[i] == (piecePosition + (isWhitesTurn ? +8 : -8))) {
        piecePositions[i] = piecePosition;
        return;
      } else if (piecePositions[i] ==
                 (piecePosition + (isWhitesTurn ? +16 : -16))) {
        piecePositions[i] = piecePosition;
        return;
      }
    }
  }
  if (strlen((move)) == 3) {
    if (move[0] == 'K') {
      piecePositions[(isWhitesTurn ? 28 : 4)] = piecePosition;
      return;
    } else if (move[0] == 'Q') {
      return;
      piecePositions[(isWhitesTurn ? 27 : 3)] = piecePosition;
    } else if (move[0] == 'R') {
      for (unsigned int i = (isWhitesTurn ? 24 : 0);
           i <= (isWhitesTurn ? 31 : 7); i += 7) {
        for (int j = -7; j < 8; j++) {
          if (piecePositions[i] == piecePosition + j ||
              piecePositions[i] == piecePosition + (8 * j)) {
            piecePositions[i] = piecePosition;
            return;
          }
        }
      }
    } else if (move[0] == 'B') {
      (((((piecePosition + 1) % 8) + ((piecePosition + 1) / 8)) % 2 == 0)
           ? (isWhitesTurn ? piecePositions[26] : piecePositions[2])
           : (isWhitesTurn ? piecePositions[29] : piecePositions[5])) =
          piecePosition;
      return;
    } else if (move[0] == 'N') {
      for (unsigned int i = (isWhitesTurn ? 25 : 1);
           i <= (isWhitesTurn ? 30 : 6); i += 5) {
        for (int j = 1; j >= -1; j -= 2) {
          if (piecePositions[i] == (piecePosition + (17 * j)) ||
              piecePositions[i] == (piecePosition + (15 * j)) ||
              piecePositions[i] == (piecePosition + (10 * j)) ||
              piecePositions[i] == (piecePosition + (8 * j))) {
            std::cout << "Knight move!" << std::endl;
            piecePositions[i] = piecePosition;
            return;
          }
        }
      }
    }
  }
}
