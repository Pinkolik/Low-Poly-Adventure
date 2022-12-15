//
// Created by pinkolik on 12/15/22.
//

#include "GameInstance.h"
#include <iostream>

GameInstance::GameInstance(const char *mapModelPath, const char *playerModelPath, const int width,
                           const int height) : width(width), height(height) {
    map = new Map(mapModelPath);
    player = new Player(playerModelPath, map->getSpawnPos());
}

void GameInstance::draw() {
    shader.use();
    shader.setBool("debug", false);
    glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
    shader.setMatrix4f("projection", projection);

    glm::mat4 view = player->getViewMatrix();
    shader.setMatrix4f("view", view);

    map->getModel().draw(shader);

    shader.setBool("debug", true);
    player->getModel().draw(shader);
    shader.setBool("debug", false);
}

void GameInstance::tick(GLFWwindow *window, const float deltaTime) {
    glm::vec3 gravity = glm::vec3(0, GRAVITY * fallTime * fallTime, 0);
    std::cout << "Fall time: " << fallTime << std::endl;
    glm::vec3 move = player->processKeyboard(window, deltaTime);
    player->applyForce(gravity);
    player->applyForce(move);

    bool first = true;
    while (true) {
        glm::vec3 *mtv = map->getModel().getMinimumTranslationVec(player->getModel(), gravity);
        if (mtv != nullptr) {
            std::cout << "Applying force: " << mtv->x << ", " << mtv->y << ", " << mtv->z << std::endl;
            player->applyForce(*mtv);
            delete mtv;
            fallTime = 0.1f;
        } else if (first) {
            fallTime += deltaTime;
            first = false;
        }
        mtv = map->getModel().getMinimumTranslationVec(player->getModel(), move);
        if (mtv != nullptr) {
            std::cout << "Applying force: " << mtv->x << "," << mtv->y << "," << mtv->z << std::endl;
            player->applyForce(*mtv);
            delete mtv;
        } else {
            break;
        }
    }
}

void GameInstance::processMouseMovement(float xOffset, float yOffset) {
    player->processMouseMovement(xOffset, yOffset);
}

void GameInstance::processResize(const int newWidth, const int newHeight) {
    width = newWidth;
    height = newHeight;
}
