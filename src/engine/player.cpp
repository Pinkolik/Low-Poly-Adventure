#include "player.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), position(position), worldUp(up), yaw(yaw),
      pitch(pitch),
      model(Model(
          "/home/pinkolik/Personal/game/resources/models/player/player.gltf")) {

  updatePlayerVectors();
}

glm::mat4 Player::getViewMatrix() {
  return glm::lookAt(position - front * 2.0f, position + front, worldUp);
}

glm::vec3 Player::getPosition() { return position; }

glm::vec3 Player::getFront() { return front; }

void Player::processKeyboard(GLFWwindow *window, Model &map, float deltaTime) {
  float velocity = movementSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += front * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= front * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= right * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += right * velocity;
  }
}

void Player::processMouseMovement(float xOffset, float yOffset) {
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  } else if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  updatePlayerVectors();
}

void Player::updatePlayerVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  right = glm::normalize(glm::cross(front, worldUp));
}

Model &Player::getModel() {
  model.setTranslation(position);
  model.setRotation(glm::quat(glm::vec3(0, glm::radians(-yaw), 0)));
  return model;
}
