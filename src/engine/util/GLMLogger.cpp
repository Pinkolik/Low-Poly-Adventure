//
// Created by pinkolik on 2/7/23.
//

#include "GLMLogger.h"
#include <iostream>
#include <iomanip>

void GLMLogger::logVec3(const std::string &prefix, const glm::vec3 &vec) {
    std::cout << std::left << std::setw(15) << prefix << ": "
              << std::left << std::setw(15) << vec.x << ","
              << std::left << std::setw(15) << vec.y << ","
              << std::left << std::setw(15) << vec.z << std::endl;
}

void GLMLogger::logMat4(const std::string &prefix, const glm::mat4 &mat) {
    std::cout << prefix << ": " << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::left << std::setw(20) << mat[j][i] << " ";
        }
        std::cout << std::endl;
    }
}
