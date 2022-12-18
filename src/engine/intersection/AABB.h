//
// Created by pinkolik on 12/18/22.
//

#ifndef GAME_AABB_H
#define GAME_AABB_H


#include "glm/vec3.hpp"

class AABB {
public:
    AABB(glm::vec3 min, glm::vec3 max);

private:
    glm::vec3 min, max;
};


#endif //GAME_AABB_H
