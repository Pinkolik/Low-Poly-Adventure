// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
// Engine
#include "engine/model.h"
#include "engine/player.h"
#include "engine/shader.h"
// GLM
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
// C++
#include <exception>
#include <iostream>
#include <stdexcept>

int width = 1600;
int height = 900;
Player *player;

void resizeCallback(GLFWwindow *window, int newWidth, int newHeight) {
  glViewport(0, 0, newWidth, newHeight);
  width = newHeight;
  height = newHeight;
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
  player->processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {}

GLFWwindow *createWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Untitled Game", NULL, NULL);
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

void processInput(GLFWwindow *window, Model &map, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  player->processKeyboard(window, map, deltaTime);
}

void mainLoop(GLFWwindow *window) {
  Shader shader =
      Shader("/home/pinkolik/Personal/game/resources/shaders/vShader.glsl",
             "/home/pinkolik/Personal/game/resources/shaders/fShader.glsl");

  Model map =
      Model("/home/pinkolik/Personal/game/resources/models/mall/mall.gltf");
  player = new Player(map.getSpawnPos());
  map.buffer();
  player->getModel().buffer();

  // timing
  float deltaTime = 0.0f; // time between current frame and last frame
  float lastFrame = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, map, deltaTime);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setBool("debug", false);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    shader.setMatrix4f("projection", projection);

    glm::mat4 view = player->getViewMatrix();
    shader.setMatrix4f("view", view);

    map.draw(shader);

    player->getModel().draw(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main() {
  try {
    GLFWwindow *window = createWindow();
    initGlad();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainLoop(window);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  return 0;
}
