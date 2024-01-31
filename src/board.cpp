//Generates vertices for 64 squares
void generateVertices(float vertices[1024]) {
  float xPosition = -1.0f;
  float yPosition = 1.25f;
  unsigned int j = 0;

  for (unsigned int i = 0; i < 64; i++) {
    if (i % 8 != 0) {
      xPosition += 0.25f;
    } else {
      xPosition = -1.0f;
      yPosition -= 0.25f;
    }
    
    //Alternates between position and texture vertices
    vertices[i + 0 + j] = xPosition;
    vertices[i + 1 + j] = yPosition;

    vertices[i + 2 + j] = 1.0f;
    vertices[i + 3 + j] = 1.0f;

    vertices[i + 4 + j] = xPosition + 0.25f;
    vertices[i + 5 + j] = yPosition;

    vertices[i + 6 + j] = 0.0f;
    vertices[i + 7 + j] = 1.0f;

    vertices[i + 8 + j] = xPosition;
    vertices[i + 9 + j] = yPosition - 0.25;

    vertices[i + 10 + j] = 1.0f;
    vertices[i + 11 + j] = 0.0f;

    vertices[i + 12 + j] = xPosition + 0.25f;
    vertices[i + 13 + j] = yPosition - 0.25;

    vertices[i + 14 + j] = 0.0f;
    vertices[i + 15 + j] = 0.0f;

    j += 15;
  }
}

//Generates indices for the squares
void generateIndices(unsigned int indices[384]) {
  int j = 0;
  for (unsigned int i = 0; i < 384; i++) {
    indices[i] = j;
    (i - 2) % 6 != 0 ? j++ : j--;
  }
}
