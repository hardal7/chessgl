#include <iostream>

// Converts FEN to a piece and vertex array
void fenToPosition(float pieceVertices[512], int piecePositions[32],
                   char fenString[90] =
                       (char *)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {
  float xPosition = -0.75f;
  float yPosition = 1.0f;

  unsigned int l = 0;
  unsigned int k = 0;

  for (unsigned int i = 0; fenString[i] != 0; i++) {
    // Check for empty squares
    if (fenString[i] == '8' || fenString[i] == '7' || fenString[i] == '6' ||
        fenString[i] == '5' || fenString[i] == '4' || fenString[i] == '3' ||
        fenString[i] == '2' || fenString[i] == '1' || fenString[i] == '/') {
      if (fenString[i] == '/') {
        xPosition = -0.75f;
        yPosition -= 0.25f;
      } else {
        int j = fenString[i] - '0';
        while (j > 0) {
          xPosition += 0.25f;
          j--;
        }
      }
    }
    // Check for pieces
    else {
      fenString[i] == 'P'   ? piecePositions[l] = 0
      : fenString[i] == 'N' ? piecePositions[l] = 1
      : fenString[i] == 'B' ? piecePositions[l] = 2
      : fenString[i] == 'R' ? piecePositions[l] = 3
      : fenString[i] == 'Q' ? piecePositions[l] = 4
      : fenString[i] == 'K' ? piecePositions[l] = 5
      : fenString[i] == 'p' ? piecePositions[l] = 6
      : fenString[i] == 'n' ? piecePositions[l] = 7
      : fenString[i] == 'b' ? piecePositions[l] = 8
      : fenString[i] == 'r' ? piecePositions[l] = 9
      : fenString[i] == 'q' ? piecePositions[l] = 10
      : fenString[i] == 'k' ? piecePositions[l] = 11
                            : -1;

      pieceVertices[l + 0 + k] = xPosition;
      pieceVertices[l + 1 + k] = yPosition;
      pieceVertices[l + 2 + k] = 1.0f, pieceVertices[l + 3 + k] = 1.0f,

      pieceVertices[l + 4 + k] = xPosition - 0.25f;
      pieceVertices[l + 5 + k] = yPosition;
      pieceVertices[l + 6 + k] = 0.0f, pieceVertices[l + 7 + k] = 1.0f,

      pieceVertices[l + 8 + k] = xPosition;
      pieceVertices[l + 9 + k] = yPosition - 0.25;
      pieceVertices[l + 10 + k] = 1.0f, pieceVertices[l + 11 + k] = 0.0f,

      pieceVertices[l + 12 + k] = xPosition - 0.25f;
      pieceVertices[l + 13 + k] = yPosition - 0.25;
      pieceVertices[l + 14 + k] = 0.0f, pieceVertices[l + 15 + k] = 0.0f,

      xPosition += 0.25f;
      l++;
      k += 15;
    }
  }
  // for (int i = 1; i <= l * 16; i++) {
  //   if ((i - 1) % 16 == 0) {
  //     std::cout << std::endl;
  //   }
  //   i % 2 ? std::cout << "(" << pieceVertices[i - 1] << ", "
  //         : std::cout << pieceVertices[i - 1] << ")" << std::endl;
  // }

  // for (int i = 1; i <= l; i++) {
  // std::cout << piecePositions[i - 1] << std::endl;
  // }
}
