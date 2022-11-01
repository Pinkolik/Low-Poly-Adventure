// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
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
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main() {
  GLFWwindow *window = createWindow();
  initGlad();
  glViewport(0, 0, WIDTH, HEIGHT);

  mainLoop(window);
  return 0;
}