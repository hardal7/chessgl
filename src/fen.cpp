void fenToPosition(
    unsigned int piecePositions[64],
    char *fenString = (char *)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {

  unsigned int l = 0;
  // i is for iterating the fenString while l is for iterating the game board

  for (unsigned int i = 0; i < 64; i++) {
    // Fill array with 0 on empty squares
    if (fenString[i] == '8' || fenString[i] == '7' || fenString[i] == '6' ||
        fenString[i] == '5' || fenString[i] == '4' || fenString[i] == '3' ||
        fenString[i] == '2' || fenString[i] == '1' || fenString[i] == '/') {
      for (int j = fenString[i] - '0'; j > 0; j--) {
        piecePositions[i] = 0;
        l++;
      }
    }

    // Fill array with piece values
    else {
      fenString[i] == 'P'   ? piecePositions[l] = 1
      : fenString[i] == 'N' ? piecePositions[l] = 2
      : fenString[i] == 'B' ? piecePositions[l] = 3
      : fenString[i] == 'R' ? piecePositions[l] = 4
      : fenString[i] == 'Q' ? piecePositions[l] = 5
      : fenString[i] == 'K' ? piecePositions[l] = 6
      : fenString[i] == 'p' ? piecePositions[l] = 7
      : fenString[i] == 'n' ? piecePositions[l] = 8
      : fenString[i] == 'b' ? piecePositions[l] = 9
      : fenString[i] == 'r' ? piecePositions[l] = 10
      : fenString[i] == 'q' ? piecePositions[l] = 11
      : fenString[i] == 'k' ? piecePositions[l] = 12
      : piecePositions[l] = 0;
      l++;
    }
  }
}
