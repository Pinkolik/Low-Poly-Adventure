#include "Player.h"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

glm::mat4 Player::getViewMatrix() {
    return glm::lookAt(position - front * 2.0f, position + front, worldUp);
}

glm::vec3 Player::processKeyboard(GLFWwindow *window, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    glm::vec3 res = glm::vec3(0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        res += front * velocity * glm::vec3(1.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        res -= front * velocity * glm::vec3(1.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        res -= right * velocity * glm::vec3(1.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        res += right * velocity * glm::vec3(1.0f, 0.0f, 1.0f);
    }
    return res;
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

void Player::applyForce(glm::vec3 &force) { position += force; }

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
    return model;
}

Player::Player(const char *playerModelPath, glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY),
        position(position),
        worldUp(up),
        yaw(yaw),
        pitch(pitch),
        model(Model(playerModelPath)) {

    updatePlayerVectors();
    model.buffer();
}
