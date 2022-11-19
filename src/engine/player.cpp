#include "player.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  updatePlayerVectors();
}

glm::mat4 Player::getViewMatrix() {
  return glm::lookAt(getEyePos(), getEyePos() + front, worldUp);
}

glm::vec3 Player::getPosition() { return position; }

glm::vec3 Player::getFront() { return front; }

float Player::getZoom() { return zoom; }

void Player::processKeyboard(GLFWwindow *window, float deltaTime) {
  float velocity = movementSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += glm::normalize(front * glm::vec3(1.0f, 0, 1.0f)) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= glm::normalize(front * glm::vec3(1.0f, 0, 1.0f)) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= right * glm::vec3(1.0f, 0, 1.0f) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += right * glm::vec3(1.0f, 0, 1.0f) * velocity;
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

void Player::processMouseScroll(float yOffset) {
  zoom -= (float)yOffset;
  if (zoom < 1.0f) {
    zoom = 1.0f;
  } else if (zoom > 45.0f) {
    zoom = 45.0f;
  }
}

void Player::updatePlayerVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  right = glm::normalize(glm::cross(front, worldUp));
}

void Player::tick(Map &map, float deltaTime) {
  glm::vec3 down = -worldUp;

  glm::vec3 afterFallPos = position + down * fallTime * GRAVITY;

  glm::vec3 *intersection = map.findIntersection(position, down);
  if (intersection == NULL) {
    position = afterFallPos;
    fallTime += deltaTime;
    return;
  }
  float fallDistance = glm::distance(position, afterFallPos);
  float intersectionDistance = glm::distance(position, *intersection);
  if (intersectionDistance < fallDistance) {
    position = *intersection;
    fallTime = 0;
  } else {
    position = afterFallPos;
    fallTime += deltaTime;
  }
  delete intersection;
}

glm::vec3 Player::getEyePos() { return position + worldUp; }