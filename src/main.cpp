#include <iostream>

#include "../includes/glad/glad.h"
#include "../includes/glfw/glfw3.h"

// To load images as textures
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

// Utility header to load shader files
#include "../includes/shader.h"

#include "board.cpp"
#include "fen.cpp"
#include "move.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void generatePieceTextures();

const int screenWidth = 640;
const int screenHeight = 640;

// This is an array of piece positions, for example if you want to get the piece
// at position 23 you could use piecePositions[22], the possible values in the
// array is from 0 to 11 each number representing a different piece
unsigned int piecePositions[64];

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Initialize GLFW
  GLFWwindow *window =
      glfwCreateWindow(screenWidth, screenHeight, "ChessGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSwapInterval(1);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 1;
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader whiteSquareShader("board.vs", "whiteSquare.fs");
  Shader blackSquareShader("board.vs", "blackSquare.fs");
  Shader pieceShader("piece.vs", "piece.fs");

  // Both the board squares and the pieces share the same vertices and indices,
  // these empty arrays are filled by their respective functions, see: board.cpp
  float vertices[1024];
  unsigned int indices[384];

  generateVertices(vertices);
  generateIndices(indices);

  unsigned int VBO[1], VAO[1], EBO[1];
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);

  // Same VAO will be used for both the board and the pieces
  // Because everything is a square, see: board.cpp
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // Position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Texture attribute (Although not useful for board squares)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  generatePieceTextures();
  pieceShader.use();
  pieceShader.setInt("pieceTexture", 0);

  // Initialize the board using a FEN value
  fenToPosition(piecePositions);

  // Render Loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.40f, 0.10, 0.40f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO[0]);

    // Alternate between white and black squares
    bool j = true;
    for (unsigned long i = 0; i < 64; i++) {
      j ? i % 2 == 0 ? whiteSquareShader.use() : blackSquareShader.use()
      : i % 2 == 0 ? blackSquareShader.use() : whiteSquareShader.use();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * i));
      (i + 1) % 8 == 0 ? j = !j : j = j;
    }

    // Render pieces using the piecePositions array, textures of the pieces are
    // determined by their values in the array, example: if pieceTexture[22] = 3
    // then the piece on the 21st square will be a white bishop
    // The pieces also have 24 vertices, see: board.cpp
    pieceShader.use();
    for (unsigned long i = 0; i < 64; i++) {
      if (piecePositions[i] != 0) {
        glBindTexture(GL_TEXTURE_2D, piecePositions[i] - 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * i));
      }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    processInput(window);
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Is called on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Generates piece textures
void generatePieceTextures() {
  const std::string pieceNames[12] = {
      "pw", "nw", "bw", "rw", "qw", "kw", //
      "pb", "nb", "bb", "rb", "qb", "kb"  //
  };

  unsigned int pieceTextures;
  int width, height, nrChannels;

  for (unsigned int i = 0; i < 12; i++) {
    glGenTextures(1, &i + 1);
    glBindTexture(GL_TEXTURE_2D, i);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data =
      stbi_load(std::string(("assets/textures/pieces/") + pieceNames[i] + std::string(".png")).c_str(),
      &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } 
    else {
      std::cout << "Failed to load texture " << pieceNames[i] << std::endl;
    }
    stbi_image_free(data);
  }
}
