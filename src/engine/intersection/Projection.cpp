#include "Projection.h"
#include <cmath>
#include <glm/glm.hpp>

Projection::Projection(glm::vec3 &axis, glm::vec3 *triangle) {
    for (int i = 0; i < 3; i++) {
        glm::vec3 point = triangle[i];
        float proj = glm::dot(axis, point);
        if (proj < min) {
            min = proj;
        }
        if (proj > max) {
            max = proj;
        }
    }
}

bool Projection::isIntersecting(Projection &other) const {
    return min <= other.max && max >= other.min || other.min <= max && other.max >= min;
}

float Projection::findIntersectionLength(Projection &other) const {
    if (min <= other.max) {
        return max - other.min;
    } else if (other.min <= max) {
        return other.max - min;
    } else {
        return -INFINITY;
    }
}