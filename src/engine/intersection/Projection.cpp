#include "Projection.h"
#include <cmath>
#include <glm/glm.hpp>

Projection::Projection(glm::vec3 axis, std::vector<glm::vec3> &triangle) {
    for (auto &point: triangle) {
        float proj = glm::dot(axis, point);
        if (proj < min) {
            min = proj;
        }
        if (proj > max) {
            max = proj;
        }
    }
}

bool Projection::isIntersecting(Projection &other) {
    return min < other.max && max >= other.min || other.min < max && other.max >= min;
}

float Projection::findIntersectionLength(Projection &other) {
    if (min < other.max) {
        return fabs(max - other.min);
    } else if (other.min < max) {
        return fabs(other.max - min);
    } else {
        return INFINITY;
    }
}