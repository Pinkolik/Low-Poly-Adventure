//
// Created by pinkolik on 12/18/22.
//

#include <iostream>
#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
}


bool AABB::isIntersecting(const AABB &other) const {
    return min.x <= other.max.x && max.x >= other.min.x &&
           min.y <= other.max.y && max.y >= other.min.y &&
           min.z <= other.max.z && max.z >= other.min.z;
}

bool AABB::isInside(glm::vec3 *triangle) const {
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

void AABB::applyTranslation(glm::mat4 transMat) {
    min = transMat * glm::vec4(min, 1.0f);
    max = transMat * glm::vec4(max, 1.0f);
}
