// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
// Helpers
#include "helpers/camera.h"
#include "helpers/map.h"
#include "helpers/model.h"
#include "helpers/shader.h"
#include "helpers/texture.h"
// GLM
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
// C++
#include <exception>
#include <iostream>
#include <stdexcept>

const int WIDTH = 1600;
const int HEIGHT = 900;

Camera camera = Camera();

void resizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

bool firstMouse = true;
float lastX, lastY;
void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;
  lastX = xPos;
  lastY = yPos;
  camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
  camera.processMouseScroll(yOffset);
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
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void initGlad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }
}

void processInput(GLFWwindow *window, float deltaTime, Camera *camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->processKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->processKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->processKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->processKeyboard(RIGHT, deltaTime);
  }
}

void mainLoop(GLFWwindow *window) {
  Shader shader = Shader("./resources/shaders/vShader.glsl",
                         "./resources/shaders/fShader.glsl");
  Texture texture = Texture("./resources/textures/wall.jpg");
  Map map = Map("./resources/maps/map1.json");

  // timing
  float deltaTime = 0.0f; // time between current frame and last frame
  float lastFrame = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime, &camera);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setInt("tex", 0);

    glm::mat4 projection =
        glm::perspective(glm::radians(camera.getZoom()),
                         (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    shader.setMatrix4f("projection", projection);

    glm::mat4 view = camera.getViewMatrix();
    shader.setMatrix4f("view", view);

    map.draw(&shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main() {
  try {
    GLFWwindow *window = createWindow();
    initGlad();
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    mainLoop(window);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  return 0;
}