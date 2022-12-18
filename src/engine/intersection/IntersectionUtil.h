#ifndef INTERSECTION_UTIL_H
#define INTERSECTION_UTIL_H

#include <glm/vec3.hpp>
#include <vector>

class IntersectionUtil {
public:
    static glm::vec3 *
    getMinimumTranslationVec(std::vector<glm::vec3> &firstTriangle,
                             glm::vec3 &firstTriangleNormal,
                             std::vector<glm::vec3> &secondTriangle,
                             glm::vec3 &secondTriangleNormal);

    static bool isLess(glm::vec3 &first, glm::vec3 &second);

    static bool isGreater(glm::vec3 &first, glm::vec3 &second);

private:
    static std::vector<glm::vec3>
    getSeparatingAxes(std::vector<glm::vec3> &triangle,
                      glm::vec3 &normal);
};

#endif