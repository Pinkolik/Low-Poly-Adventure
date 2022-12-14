#ifndef INTERSECTION_UTIL_H
#define INTERSECTION_UTIL_H

#include <glm/vec3.hpp>
#include <vector>

class IntersectionUtil {
public:
    static glm::vec3 *
    getMinimumTranslationVec(std::vector<glm::vec3> &firstTriangle,
                             std::vector<glm::vec3> &firstTriangleNormals,
                             std::vector<glm::vec3> &secondTriangle,
                             std::vector<glm::vec3> &secondTriangleNormals);

private:
    static std::vector<glm::vec3>
    getSeparatingAxes(std::vector<glm::vec3> &triangle,
                      std::vector<glm::vec3> &normals);
};

#endif