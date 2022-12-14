#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.1f;

class Player {
public:
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH);

    void processKeyboard(GLFWwindow *window, Model &map, float deltaTime);

    void processMouseMovement(float xOffset, float yOffset);

    void applyForce(glm::vec3 &force);

    glm::mat4 getViewMatrix();

    glm::vec3 getPosition();

    glm::vec3 getFront();

    Model &getModel();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float fallTime;
    float lastPressTime = 0;

    bool thirdPerson = false;
    Model model;

    void updatePlayerVectors();
};

#endif
