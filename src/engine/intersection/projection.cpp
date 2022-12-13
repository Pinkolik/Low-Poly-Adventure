#include "projection.h"
#include <glm/glm.hpp>

Projection::Projection(glm::vec3 axis, std::vector<glm::vec3> &triangle) {
  for (auto &point : triangle) {
    float proj = glm::dot(axis, point);
    if (proj < min) {
      min = proj;
    }
    if (proj > max) {
      max = proj;
    }
  }
}

float Projection::getMin() { return min; }

float Projection::getMax() { return max; }

bool Projection::isIntersecting(Projection &other) {
  if (min > other.min) {
    return min <= other.max;
  } else {
    return other.min <= other.max;
  }
}