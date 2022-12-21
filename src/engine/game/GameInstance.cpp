//
// Created by pinkolik on 12/15/22.
//

#include "GameInstance.h"
#include "../intersection/IntersectionUtil.h"
#include <iostream>

GameInstance::GameInstance(const char *mapModelPath, const char *playerModelPath, const int width,
                           const int height) : width(width), height(height) {
    map = new Map(mapModelPath);
    player = new Player(playerModelPath, map->getSpawnPos() + glm::vec3(0.0f, 1.0f, 0.0f));
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
    std::string title = "Low Poly Adventure FPS: " + std::to_string((int) floor(1.0f / deltaTime));
    glfwSetWindowTitle(window, title.c_str());
    glm::vec3 gravity = glm::vec3(0, GRAVITY * fallTime * fallTime, 0);
//    glm::vec3 gravity = glm::vec3(0, 0, 0);
//    std::cout << "Fall time: " << fallTime << std::endl;
    glm::vec3 move = player->processKeyboard(window, deltaTime);
    player->applyForce(gravity);
    player->applyForce(move);

    bool first = true;
    while (true) {
        //first gravity
        std::vector<IntersectionResult *> intersections = map->getModel().getMinimumTranslationVec(player->getModel());
        glm::vec3 *mtv = IntersectionUtil::getMostOppositeVec(intersections, gravity);
        if (mtv != nullptr) {
            std::cout << "Applying force: " << mtv->x << ", " << mtv->y << ", " << mtv->z << std::endl;
            glm::vec3 force = *mtv;
            player->applyForce(force);
            fallTime = 0.0f;
        } else if (first) {
            fallTime += deltaTime;
            first = false;
        }
        intersections = IntersectionUtil::recalculateIntersections(intersections, mtv);
        //second movement
        mtv = IntersectionUtil::getMostOppositeVec(intersections, move);
        if (mtv != nullptr) {
            std::cout << "Applying force: " << mtv->x << "," << mtv->y << "," << mtv->z << std::endl;
            player->applyForce(*mtv);
        } else {
            break;
        }
        for (auto &item: intersections) {
            delete item;
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
