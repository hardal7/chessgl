#include "../includes/glad/glad.h"
#include "../includes/glfw/glfw3.h"
#include "../includes/glm/gtc/type_ptr.hpp"
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader whiteSquareShader("assets/shaders/board.vs",
                           "assets/shaders/whiteSquare.fs");
  Shader blackSquareShader("assets/shaders/board.vs",
                           "assets/shaders/blackSquare.fs");
  Shader pieceShader("assets/shaders/piece.vs", "assets/shaders/piece.fs");

  float whiteSquareVertices[256];
  generateVertices(whiteSquareVertices, 'w');
  float blackSquareVertices[256];
  generateVertices(blackSquareVertices, 'b');
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

  float pieceVertices[] = {
      // positions  // texture coords
      0.25f, 0.25f, 0.0f, 1.0f, 1.0f, // top right
      0.25f, -0.f,  0.0f, 1.0f, 0.0f, // bottom right
      -0.f,  -0.f,  0.0f, 0.0f, 0.0f, // bottom left
      -0.f,  0.25f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int pieceIndices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int pieceTexture;

  glGenTextures(1, &pieceTexture);
  glBindTexture(GL_TEXTURE_2D, pieceTexture);
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
  unsigned char *data = stbi_load("assets/textures/pieces/bw.png", &width,
                                  &height, &nrChannels, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  glBindVertexArray(VAO[2]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pieceVertices), pieceVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pieceIndices), pieceIndices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  pieceShader.use();
  pieceShader.setInt("pieceTexture", 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.40f, 0.10, 0.40f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // create transformations
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
    unsigned int blackProjectionLoc=
        glGetUniformLocation(blackSquareShader.ID, "projection");
    glUniformMatrix4fv(blackModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(blackViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(blackProjectionLoc, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pieceTexture);
    pieceShader.use();
    glBindVertexArray(VAO[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
  unsigned char *data = stbi_load("assets/textures/pieces/awesomeface.png",
                                  &width, &height, &nrChannels, 0);
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
