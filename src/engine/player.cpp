#include "player.h"
#include "debug_cube.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "utils.h"
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
  this->playerBox = new Node(Utils::createBoxNode());
  playerBox->setScale(glm::vec3(0.2, 0.55, 0.2));
  updatePlayerVectors();
}

glm::mat4 Player::getViewMatrix() {
  if (thirdPerson) {
    return glm::lookAt(getEyePos() - front * 2.0f, getEyePos() + front,
                       worldUp);
  } else {
    return glm::lookAt(getEyePos(), getEyePos() + front, worldUp);
  }
}

glm::vec3 Player::getPosition() { return position; }

glm::vec3 Player::getFront() { return front; }

float Player::getZoom() { return zoom; }

void Player::processKeyboard(GLFWwindow *window, Map &map, float deltaTime) {
  float pressTime = glfwGetTime();
  float velocity = movementSpeed * deltaTime;
  glm::vec3 direction = glm::vec3(0);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    direction += glm::normalize(front * glm::vec3(1.0f, 0, 1.0f)) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    direction -= glm::normalize(front * glm::vec3(1.0f, 0, 1.0f)) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    direction -= right * glm::vec3(1.0f, 0, 1.0f) * velocity;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    direction += right * glm::vec3(1.0f, 0, 1.0f) * velocity;
  }
  float intersectionCoefficient =
      map.findIntersectionCoefficient(*getPlayerBox(), direction) - 0.001f;
  Node projection = *getPlayerBox();
  projection.setTranslation(projection.getTranslation() +
                            direction * intersectionCoefficient);
  Utils::debugNodes.push_back(projection);
  position += direction * min(1.0f, intersectionCoefficient);
  if (pressTime - lastPressTime > 0.3 &&
      glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    thirdPerson = !thirdPerson;
    lastPressTime = pressTime;
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
  float fallCoefficient = fallTime * GRAVITY;

  float intersectionCoefficient =
      map.findIntersectionCoefficient(*getPlayerBox(), down) - 0.001f;
  Node fallProjection = *getPlayerBox();
  fallProjection.setTranslation(fallProjection.getTranslation() +
                                down * intersectionCoefficient);
  Utils::debugNodes.push_back(fallProjection);

  if (fallCoefficient > intersectionCoefficient) {
    position += down * intersectionCoefficient;
    fallTime = 0;
  } else {
    position += down * fallCoefficient;
    fallTime += deltaTime;
  }
}

glm::vec3 Player::getEyePos() { return position + worldUp; }

Node *Player::getPlayerBox() {
  if (playerBox != NULL) {
    playerBox->setTranslation(position + glm::vec3(0, 0.55, 0));
    playerBox->setRotation(glm::quat(glm::vec3(0, glm::radians(-yaw), 0)));
  }
  return playerBox;
}