#include "../includes/glad/glad.h"
#include "../includes/glfw/glfw3.h"
#include "../includes/shader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"
#include "board.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void generateTexture(unsigned int texture, char texturePath[],
                     bool isTransparent);

const int screenWidth = 600;
const int screenHeight = 600;

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

  Shader whiteSquareShader("assets/shaders/board.vs",
                           "assets/shaders/whiteSquare.fs");
  Shader blackSquareShader("assets/shaders/board.vs",
                           "assets/shaders/blackSquare.fs");
  Shader pieceShader("assets/shaders/piece.vs", "assets/shaders/piece.fs");

  float whiteSquareVertices[512];
  generateVertices(whiteSquareVertices, 'w');
  float blackSquareVertices[512];
  generateVertices(blackSquareVertices, 'b');
  unsigned int indices[384];
  generateIndices(indices);

  unsigned int VBO[2], VAO[2], EBO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);
  glGenBuffers(2, EBO);

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

  // render loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.40f, 0.10, 0.40f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    whiteSquareShader.use();
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 384, GL_UNSIGNED_INT, 0);

    blackSquareShader.use();
    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 384, GL_UNSIGNED_INT, 0);

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

void generateTexture(unsigned int texture, char texturePath[],
                     bool isTransparent) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data) {
    if (isTransparent)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}
