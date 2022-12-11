#include "intersection_util.h"
#include "projection.h"
#include <glm/glm.hpp>
#include <vector>

bool IntersectionUtil::isIntersecting(std::vector<glm::vec3> &firstTriangle,
                                      std::vector<glm::vec3> &secondTriangle) {
  std::vector<glm::vec3> separatingAxes;
  std::vector<glm::vec3> firstAxes = getSeparatingAxes(firstTriangle);
  std::vector<glm::vec3> secondAxes = getSeparatingAxes(secondTriangle);
  separatingAxes.insert(separatingAxes.end(), firstAxes.begin(),
                        firstAxes.end());
  separatingAxes.insert(separatingAxes.end(), secondAxes.begin(),
                        secondAxes.end());
  for (auto &axis : separatingAxes) {
    Projection firstProj = Projection(axis, firstTriangle);
    Projection secondProj = Projection(axis, secondTriangle);
    if (!firstProj.isIntersecting(secondProj)) {
      return false;
    }
  }
  return true;
}

std::vector<glm::vec3>
IntersectionUtil::getSeparatingAxes(std::vector<glm::vec3> &triangle) {
  glm::vec3 ab, bc, ca;
  ab = triangle[1] - triangle[0];
  bc = triangle[2] - triangle[1];
  ca = triangle[0] - triangle[2];

  std::vector<glm::vec3> res;
  res.push_back(glm::normalize(glm::cross(glm::cross(bc, ab), ab)));
  res.push_back(glm::normalize(glm::cross(glm::cross(ca, bc), bc)));
  res.push_back(glm::normalize(glm::cross(glm::cross(ab, ca), ca)));
  return res;
}