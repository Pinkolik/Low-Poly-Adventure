//
// Created by pinkolik on 12/18/22.
//

#ifndef GAME_AABB_H
#define GAME_AABB_H


#include <vector>
#include "glm/vec3.hpp"
#include "../../shader/Shader.h"

class AABB {
public:
    AABB(glm::vec3 min, glm::vec3 max);

    bool isIntersecting(glm::vec3 translation, AABB *other, glm::vec3 otherTranslation);

private:
    glm::vec3 min, max;
};


#endif //GAME_AABB_H
