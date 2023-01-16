//
// Created by pinkolik on 12/15/22.
//

#include "GameInstance.h"
#include "../intersection/IntersectionUtil.h"

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
    glm::vec3 gravity = glm::vec3(0, GRAVITY * fallTime * fallTime, 0);
    glm::vec3 move = player->processKeyboard(window, deltaTime);

    if (processIntersectionWithMap(gravity)) {
        fallTime = 0.0f;
    } else {
        fallTime += deltaTime;
    }

    processIntersectionWithMap(move);

}

void GameInstance::processMouseMovement(float xOffset, float yOffset) {
    player->processMouseMovement(xOffset, yOffset);
}

void GameInstance::processResize(const int newWidth, const int newHeight) {
    width = newWidth;
    height = newHeight;
}

bool GameInstance::processIntersectionWithMap(glm::vec3 move) {
    bool wasApplied = false;
    glm::vec3 *mtv;
    bool exit = false;

    player->applyForce(move);

    std::vector<glm::vec3 *> mtvs;
    while (!exit) {
        mtvs = map->getModel().getMinimumTranslationVec(player->getModel());
        mtv = IntersectionUtil::getMostOppositeVec(mtvs, move);
        if (mtv != nullptr) {
            player->applyForce(*mtv);
            wasApplied = true;
        } else {
            exit = true;
        }
        for (auto &item: mtvs) {
            delete item;
        }
    }
    return wasApplied;
}
