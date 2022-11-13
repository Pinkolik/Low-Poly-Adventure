#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Player {
public:
  Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);

  glm::mat4 getViewMatrix();

  glm::vec3 getPosition();

  glm::vec3 getFront();

  float getZoom();

  void processKeyboard(GLFWwindow *window, float deltaTime);

  void processMouseMovement(float xOffset, float yOffset);

  void processMouseScroll(float yOffset);

private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;

  float movementSpeed;
  float mouseSensitivity;
  float zoom;

  void updatePlayerVectors();
};
#endif