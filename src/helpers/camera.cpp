#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  this->position = glm::vec3(posX, posY, posZ);
  this->worldUp = glm::vec3(upX, upY, upZ);
  this->yaw = yaw;
  this->pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, worldUp);
}

glm::vec3 Camera::getPosition() { return position; }

float Camera::getZoom() { return zoom; }

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
  float velocity = movementSpeed * deltaTime;
  if (direction == FORWARD) {
    position += front * velocity;
  }
  if (direction == BACKWARD) {
    position -= front * velocity;
  }
  if (direction == LEFT) {
    position -= right * velocity;
  }
  if (direction == RIGHT) {
    position += right * velocity;
  }
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  } else if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
  zoom -= (float)yOffset;
  if (zoom < 1.0f) {
    zoom = 1.0f;
  } else if (zoom > 45.0f) {
    zoom = 45.0f;
  }
}

void Camera::updateCameraVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  right = glm::normalize(glm::cross(front, worldUp));
}