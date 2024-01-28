#include <iostream>

#include "../includes/glad/glad.h"
#include "../includes/glfw/glfw3.h"
#include "../includes/glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

#include "../includes/shader.h"
#include "board.cpp"
#include "fen.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void generatePieceTextures();

const int screenWidth = 640;
const int screenHeight = 640;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(screenWidth, screenHeight, "ChessGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 1;
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader whiteSquareShader((char *)"assets/shaders/board.vs",
                           (char *)"assets/shaders/whiteSquare.fs");
  Shader blackSquareShader((char *)"assets/shaders/board.vs",
                           (char *)"assets/shaders/blackSquare.fs");
  Shader pieceShader((char *)"assets/shaders/piece.vs",
                     "assets/shaders/piece.fs");

  float whiteSquareVertices[256];
  generateVertices(whiteSquareVertices, 'w');
  float blackSquareVertices[256];
  generateVertices(blackSquareVertices, 'b');
  float pieceVertices[512];
  int piecePositions[32];
  fenToPosition(pieceVertices, piecePositions, (char*)"5r2/8/1R6/ppk3p1/2N3P1/P4b2/1K6/5B2");
  unsigned int indices[192];
  generateIndices(indices);

  unsigned int VBO[3], VAO[3], EBO[3];
  glGenVertexArrays(3, VAO);
  glGenBuffers(3, VBO);
  glGenBuffers(3, EBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(whiteSquareVertices),
               whiteSquareVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(blackSquareVertices),
               blackSquareVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  enum Piece {
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing
  };

  generatePieceTextures();
  pieceShader.use();
  pieceShader.setInt("pieceTexture", 0);

  glBindVertexArray(VAO[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pieceVertices), pieceVertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.40f, 0.10, 0.40f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    whiteSquareShader.setMat4("projection", projection);
    blackSquareShader.setMat4("projection", projection);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    projection = glm::perspective(glm::radians(90.0f),
                                  (float)screenWidth / (float)screenHeight,
                                  0.1f, 100.0f);

    unsigned int whiteModelLoc =
        glGetUniformLocation(whiteSquareShader.ID, "model");
    unsigned int whiteViewLoc =
        glGetUniformLocation(whiteSquareShader.ID, "view");
    unsigned int blackModelLoc =
        glGetUniformLocation(blackSquareShader.ID, "model");
    unsigned int blackViewLoc =
        glGetUniformLocation(blackSquareShader.ID, "view");

    whiteSquareShader.use();
    unsigned int whiteProjectionLoc =
        glGetUniformLocation(whiteSquareShader.ID, "projection");
    glUniformMatrix4fv(whiteModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(whiteViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(whiteProjectionLoc, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);

    blackSquareShader.use();
    unsigned int blackProjectionLoc =
        glGetUniformLocation(blackSquareShader.ID, "projection");
    glUniformMatrix4fv(blackModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(blackViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(blackProjectionLoc, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);

    pieceShader.use();
    glBindVertexArray(VAO[2]);
    for (unsigned long i = 0; i < 32; i++) {
      glBindTexture(GL_TEXTURE_2D, piecePositions[i]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24*i));
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void generatePieceTextures() {
  const std::string pieceNames[12] = {
      "pw", "nw", "bw", "rw", "qw", "kw", //
      "pb", "nb", "bb", "rb", "qb", "kb"  //
  };

  unsigned int pieceTextures;
  int width, height, nrChannels;

  for (unsigned int i = 0; i < 12; i++) {
    // TODO generate all 16 textures outside of the loop
    glGenTextures(1, &i + 1);
    glBindTexture(GL_TEXTURE_2D, i);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load(std::string(("assets/textures/pieces/") + pieceNames[i] +
                              std::string(".png"))
                      .c_str(),
                  &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      // std::cout << "Generated texture(" << i << "): " << pieceNames[i]
      //           << std::endl;
    } else {
      std::cout << "Failed to load texture " << pieceNames[i] << std::endl;
    }
    stbi_image_free(data);
  }
}
