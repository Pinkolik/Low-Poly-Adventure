//
// Created by pinkolik on 12/18/22.
//

#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
}


bool AABB::isIntersecting(glm::vec3 translation, AABB *other, glm::vec3 otherTranslation) {
    glm::vec3 firstMin = translation + min;
    glm::vec3 firstMax = translation + max;
    glm::vec3 secondMin = otherTranslation + other->min;
    glm::vec3 secondMax = otherTranslation + other->max;
    return firstMin.x <= secondMax.x && firstMax.x >= secondMin.x &&
           firstMin.y <= secondMax.y && firstMax.y >= secondMin.y &&
           firstMin.z <= secondMax.z && firstMax.z >= secondMin.z;
}
