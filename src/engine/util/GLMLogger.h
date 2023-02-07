//
// Created by pinkolik on 2/7/23.
//

#ifndef GAME_GLMLOGGER_H
#define GAME_GLMLOGGER_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <string>


class GLMLogger {
public:
    static void logVec3(const std::string &prefix, const glm::vec3 &vec);

    static void logMat4(const std::string &prefix, const glm::mat4 &mat);
};


#endif //GAME_GLMLOGGER_H
