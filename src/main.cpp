#include <iostream>

#include "../includes/glad/glad.h"
#include "../includes/glfw/glfw3.h"
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

  float vertices[1024];
  unsigned int indices[384];
  unsigned int pieceNames[64];
  unsigned long piecePositions[64];

  fenToPosition(piecePositions, pieceNames);
  //              (char *)"5r2/8/1R6/ppk3p1/2N3P1/P4b2/1K6/5B2");
  generateVertices(vertices);
  generateIndices(indices);

  unsigned int VBO[1], VAO[1], EBO[1];
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  generatePieceTextures();
  pieceShader.use();
  pieceShader.setInt("pieceTexture", 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.40f, 0.10, 0.40f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO[0]);

    bool j = true;
    for (unsigned long i = 0; i < 64; i++) {
      j ? i % 2 == 0 ? whiteSquareShader.use() : blackSquareShader.use()
      : i % 2 == 0 ? blackSquareShader.use()
                   : whiteSquareShader.use();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * i));
      (i + 1) % 8 == 0 ? j = !j : j = j;
    }

    pieceShader.use();
    for (unsigned long i = 0; i < 32; i++) {
      glBindTexture(GL_TEXTURE_2D, pieceNames[i]);
      if (i == 19) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                       (void *)(24 * (piecePositions[i] - 16)));
      } else {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                       (void *)(24 * piecePositions[i]));
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
