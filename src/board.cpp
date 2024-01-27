// Generates vertices for the chessboard
void generateVertices(float vertices[256], char color) {
  float xPosition = -1.0f;
  float yPosition = 1.25f;
  unsigned int j = 0;

  for (unsigned int i = 0; i < 32; i++) {
    if (i % 4 != 0) {
      xPosition += 0.5f;
    } else {
      i % 8 != 0 ? xPosition = color == 'w' ? -0.75f : -1.0f
                 : xPosition = color == 'w' ? -1.0f : -0.75f;
      yPosition -= 0.25f;
    }

    // There are duplicate vertices, but the generateIndices function has to be
    // rewritten to fix this.
    vertices[i + 0 + j] = xPosition; // 0
    vertices[i + 1 + j] = yPosition; // 1

    vertices[i + 2 + j] = xPosition + 0.25f; // 2
    vertices[i + 3 + j] = yPosition;         // 1

    vertices[i + 4 + j] = xPosition;        // 0
    vertices[i + 5 + j] = yPosition - 0.25; // 3

    vertices[i + 6 + j] = xPosition + 0.25f; // 2
    vertices[i + 7 + j] = yPosition - 0.25;  // 3

    j += 7;
  }
  // for (int i = 1; i <= 512; i++)
  //   i % 2   ? std::cout << "(" << vertices[i-1] << ", "
  //           : std::cout << vertices[i-1] << ")"<< std::endl;
}

// Generates indices for the chessboard
void generateIndices(unsigned int indices[384]) {

  unsigned int j = 0;

  // 012 10323
  // 012121
  // j=0, j++, j++, j--, j++, j++, j--, j--, j+=2, j-=2,j+=2,j++

  for (unsigned int i = 0; i < 192; i++) {
    indices[i] = j;
    (i - 2) % 6 != 0 ? j++ : j--;
  }

  // for (int i = 1; i <= 384; i++)
  //   i % 3 != 0 ? std::cout << indices[i - 1] << ", "
  //              : std::cout << indices[i - 1] << std::endl;

  // The pattern for the indices follow:
  // 0, 1, 2, //
  // 1, 2, 3, //
  // 4, 5, 6, //
  // 5, 6, 7, //
  // 8, 9, 10,//
  // 9, 10, 11//
}
