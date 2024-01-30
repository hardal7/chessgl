void fenToPosition(
    unsigned long piecePositions[32], unsigned int pieceNames[32],
    char *fenString = (char *)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {
  unsigned int l = 0;
  unsigned int piecePosition = 0;

  for (unsigned int i = 0; fenString[i] != 0; i++) {
    // Check for empty squares
    if (fenString[i] == '8' || fenString[i] == '7' || fenString[i] == '6' ||
        fenString[i] == '5' || fenString[i] == '4' || fenString[i] == '3' ||
        fenString[i] == '2' || fenString[i] == '1' || fenString[i] == '/') {
      for (int j = fenString[i] - '0'; j > 0; j--) {
        piecePosition++;
      }
    }
    // Check for pieces
    else {
      fenString[i] == 'P'   ? pieceNames[l] = 0
      : fenString[i] == 'N' ? pieceNames[l] = 1
      : fenString[i] == 'B' ? pieceNames[l] = 2
      : fenString[i] == 'R' ? pieceNames[l] = 3
      : fenString[i] == 'Q' ? pieceNames[l] = 4
      : fenString[i] == 'K' ? pieceNames[l] = 5
      : fenString[i] == 'p' ? pieceNames[l] = 6
      : fenString[i] == 'n' ? pieceNames[l] = 7
      : fenString[i] == 'b' ? pieceNames[l] = 8
      : fenString[i] == 'r' ? pieceNames[l] = 9
      : fenString[i] == 'q' ? pieceNames[l] = 10
      : fenString[i] == 'k' ? pieceNames[l] = 11
                            : -1;
      piecePositions[l] = piecePosition;
      piecePosition++;
      l++;
    }
  }
}
