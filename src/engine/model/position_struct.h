#ifndef POSITION_STRUCT_H
#define POSITION_STRUCT_H

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

struct PositionStruct {
    glm::quat rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 translation = glm::vec3(0);
};

#endif