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

    bool isIntersecting(const AABB &other) const;

    bool isInside(glm::vec3 *triangle) const;

    const glm::vec3 &getMin() const;

    const glm::vec3 &getMax() const;

    void applyTranslation(glm::mat4 transMat);

private:
    glm::vec3 min, max;
};


#endif //GAME_AABB_H
