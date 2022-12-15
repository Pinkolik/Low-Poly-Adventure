// OpenGL linker
#include <glad/glad.h>
// Windows initialization
#include <GLFW/glfw3.h>
// Engine
#include "engine/model/Model.h"
#include "engine/player/Player.h"
#include "engine/game/GameInstance.h"
// GLM
#include <glm/fwd.hpp>
// C++
#include <exception>
#include <iostream>
#include <stdexcept>

int width = 1600;
int height = 900;
GameInstance *gameInstance;

void resizeCallback(GLFWwindow *window, int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    width = newHeight;
    height = newHeight;
    gameInstance->processResize(newWidth, newHeight);
}

bool firstMouse = true;
float lastX, lastY;

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    gameInstance->processMouseMovement(xOffset, yOffset);
}


GLFWwindow *createWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
            glfwCreateWindow(width, height, "Untitled Game", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("Failed to initialize window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void initGlad() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mainLoop(GLFWwindow *window) {
    gameInstance = new GameInstance("resources/models/mall/mall.gltf", "resources/models/player/player.gltf", width,
                                    height);

    // timing
    float deltaTime; // time between current frame and last frame
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        gameInstance->tick(window, deltaTime);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gameInstance->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    try {
        GLFWwindow *window = createWindow();
        initGlad();
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        mainLoop(window);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
