//
// Created by pinkolik on 12/15/22.
//

#ifndef GAME_GAMEINSTANCE_H
#define GAME_GAMEINSTANCE_H


#include "../map/Map.h"
#include "../player/Player.h"
#include "../../shader/Shader.h"

class GameInstance {
public:
    GameInstance(const char *mapModelPath, const char *playerModelPath, int width, int height);

    void draw();

    void tick(GLFWwindow *window, float deltaTime);

    void processMouseMovement(float xOffset, float yOffset);

    void processResize(int newWidth, int newHeight);

private:
    const float GRAVITY = -0.05f;

    int width;
    int height;

    Map *map;
    Player *player;

    float fallTime = 0.1f;

    Shader shader = Shader("resources/shaders/vShader.glsl",
                           "resources/shaders/fShader.glsl");
};


#endif //GAME_GAMEINSTANCE_H
