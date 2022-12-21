#ifndef INTERSECTION_UTIL_H
#define INTERSECTION_UTIL_H

#include <glm/vec3.hpp>
#include <vector>
#include "IntersectionResult.h"

class IntersectionUtil {
public:
    static IntersectionResult *
    getMinimumTranslationVec(std::vector<glm::vec3> &firstTriangle, glm::vec3 &firstTriangleNormal,
                             std::vector<glm::vec3> &secondTriangle, glm::vec3 &secondTriangleNormal);

    static std::vector<IntersectionResult *>
    recalculateIntersections(std::vector<IntersectionResult *> &intersections,
                             glm::vec3 *translationForSecond);

    static glm::vec3 updateIfLess(glm::vec3 &first, glm::vec3 &second);

    static glm::vec3 updateIfGreater(glm::vec3 &first, glm::vec3 &second);

    static glm::vec3 *getMostOppositeVec(std::vector<IntersectionResult *> &intersections, glm::vec3 direction);

private:
    static std::vector<glm::vec3>
    getSeparatingAxes(std::vector<glm::vec3> &triangle,
                      glm::vec3 &normal);
};

#endif