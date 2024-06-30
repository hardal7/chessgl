#include <iostream>
void fenToPosition(
    unsigned int piecePositions[64],
    std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {

  // 0th position is top left, 63rd position is bottom right
  unsigned int squareIndex = 0;

  // i is for iterating the fenString
  // "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1")
  for (unsigned int i = 0; i < fenString.length(); i++) {
    // Fill empty squares of the board array
    if (fenString[i] == '8' || fenString[i] == '7' || fenString[i] == '6' ||
        fenString[i] == '5' || fenString[i] == '4' || fenString[i] == '3' ||
        fenString[i] == '2' || fenString[i] == '1' || fenString[i] == '/') {
      for (int j = fenString[i] - '0'; j > 0; j--) {
        piecePositions[squareIndex] = 0;
        squareIndex++;
      }
    }

    // Fill board array with piece values
    else {
      fenString[i] == 'P'   ? piecePositions[squareIndex] = 1
      : fenString[i] == 'N' ? piecePositions[squareIndex] = 2
      : fenString[i] == 'B' ? piecePositions[squareIndex] = 3
      : fenString[i] == 'R' ? piecePositions[squareIndex] = 4
      : fenString[i] == 'Q' ? piecePositions[squareIndex] = 5
      : fenString[i] == 'K' ? piecePositions[squareIndex] = 6

      : fenString[i] == 'p' ? piecePositions[squareIndex] = 7
      : fenString[i] == 'n' ? piecePositions[squareIndex] = 8
      : fenString[i] == 'b' ? piecePositions[squareIndex] = 9
      : fenString[i] == 'r' ? piecePositions[squareIndex] = 10
      : fenString[i] == 'q' ? piecePositions[squareIndex] = 11
      : fenString[i] == 'k' ? piecePositions[squareIndex] = 12
                            : piecePositions[squareIndex] = 0;

      squareIndex++;
    }
  }
}
