// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
// Helpers
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "helpers/camera.h"
#include "helpers/shader.h"
#include "helpers/texture.h"
#include <exception>
#include <iostream>
#include <stdexcept>

const int WIDTH = 800;
const int HEIGHT = 600;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

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

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(RIGHT, deltaTime);
  }
}

void mainLoop(GLFWwindow *window) {
  Shader shader = Shader("./resources/shaders/vShader.glsl",
                         "./resources/shaders/fShader.glsl");
  Texture texture = Texture("./resources/textures/wall.jpg");

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // lower left
                      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  // upper left
                      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // upper right
                      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f}; // lower right
  unsigned int indices[] = {0, 1, 2,                   // fisrt triangle
                            2, 3, 0};                  // second triangle

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    texture.active();
    shader.setInt("tex", 0);

    glm::mat4 projection =
        glm::perspective(glm::radians(camera.getZoom()),
                         (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    shader.setMatrix4f("projection", projection);

    // glm::mat4 view = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1),
    // glm::vec3(0,1,0));
    glm::mat4 view = camera.getViewMatrix();
    shader.setMatrix4f("view", view);

    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, currentFrame * glm::radians(20.0f),
                        glm::vec3(1, 0, 0));
    shader.setMatrix4f("model", model);

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