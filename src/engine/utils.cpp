#include "utils.h"

glm::vec3 *Utils::getMinDistanceToOriginVector(vector<glm::vec3 *> &points,
                                               glm::vec3 origin) {
  glm::vec3 *result = NULL;
  if (points.empty()) {
    return NULL;
  }
  float minDistance = MAXFLOAT;
  for (auto point : points) {
    float distance = glm::distance(*point, origin);
    if (distance < minDistance) {
      minDistance = distance;
      delete result;
      result = point;
    } else {
      delete point;
    }
  }
  return result;
}

bool Utils::isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                                  glm::vec3 normal, glm::vec3 point) {
  glm::vec3 edge0 = b - a;
  glm::vec3 edge1 = c - b;
  glm::vec3 edge2 = a - c;
  glm::vec3 c0 = point - a;
  glm::vec3 c1 = point - b;
  glm::vec3 c2 = point - c;

  return glm::dot(normal, glm::cross(edge0, c0)) > 0 &&
         glm::dot(normal, glm::cross(edge1, c1)) > 0 &&
         glm::dot(normal, glm::cross(edge2, c2)) > 0;
}
