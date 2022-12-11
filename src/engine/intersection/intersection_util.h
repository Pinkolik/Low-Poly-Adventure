#ifndef INTERSECTION_UTIL_H
#define INTERSECTION_UTIL_H
#include <glm/vec3.hpp>
#include <vector>

class IntersectionUtil {
public:
  static bool isIntersecting(std::vector<glm::vec3> &firstTriangle,
                             std::vector<glm::vec3> &secondTriangle);

private:
  static std::vector<glm::vec3>
  getSeparatingAxes(std::vector<glm::vec3> &triangle);
};

#endif