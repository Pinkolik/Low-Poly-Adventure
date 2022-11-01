// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
// Helpers
#include "helpers/shader.h"
#include <exception>
#include <iostream>
#include <stdexcept>

const int WIDTH = 800;
const int HEIGHT = 600;

void resizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow *createWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Untitled Game", NULL, NULL);
  if (window == NULL) {
    throw std::runtime_error("Failed to initialize window");
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  return window;
}

void initGlad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }
}

void mainLoop(GLFWwindow *window) {
  Shader shader = Shader("./resources/shaders/vShader.glsl",
                         "./resources/shaders/fShader.glsl");

  float vertices[] = {-0.5f, -0.5f, 0.0f,  // lower left
                      -0.5f, 0.5f,  0.0f,  // upper left
                      0.5f,  0.5f,  0.0f,  // upper right
                      0.5f,  -0.5f, 0.0f}; // lower right
  unsigned int indices[] = {0, 1, 2,       // fisrt triangle
                            2, 3, 0};      // second triangle

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main() {
  try {
    GLFWwindow *window = createWindow();
    initGlad();
    glViewport(0, 0, WIDTH, HEIGHT);

    mainLoop(window);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  return 0;
}