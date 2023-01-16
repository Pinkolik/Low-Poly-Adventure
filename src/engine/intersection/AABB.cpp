//
// Created by pinkolik on 12/18/22.
//

#include <iostream>
#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
}


bool AABB::isIntersecting(glm::mat4 transMat, AABB *other, glm::mat4 otherTransMat) {
    glm::vec3 firstMin = transMat * glm::vec4(min, 1.0f);
    glm::vec3 firstMax = transMat * glm::vec4(max, 1.0f);
    glm::vec3 secondMin = otherTransMat * glm::vec4(other->min, 1.0f);
    glm::vec3 secondMax = otherTransMat * glm::vec4(other->max, 1.0f);
    return firstMin.x <= secondMax.x && firstMax.x >= secondMin.x &&
           firstMin.y <= secondMax.y && firstMax.y >= secondMin.y &&
           firstMin.z <= secondMax.z && firstMax.z >= secondMin.z;
}

bool AABB::isInside(glm::vec3 *triangle) {
    for (int i = 0; i < 3; i++) {
        glm::vec3 &point = triangle[i];
        if (!(point.x >= min.x && point.x <= max.x &&
              point.y >= min.y && point.y <= max.y &&
              point.z >= min.z && point.z <= max.z)) {
            return false;
        }
    }
    return true;
}

const glm::vec3 &AABB::getMin() const {
    return min;
}

const glm::vec3 &AABB::getMax() const {
    return max;
}
