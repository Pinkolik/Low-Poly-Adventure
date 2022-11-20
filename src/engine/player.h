#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float GRAVITY = 0.1;

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

  void tick(Map &map, float deltaTime);

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
  float fallTime;
  float lastPressTime = 0;

  bool thirdPerson = false;

  glm::vec3 getEyePos();
  void updatePlayerVectors();
};
#endif