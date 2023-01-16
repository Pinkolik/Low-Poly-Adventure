#ifndef INTERSECTION_UTIL_H
#define INTERSECTION_UTIL_H

#include <glm/vec3.hpp>
#include <vector>

class IntersectionUtil {
public:
    static glm::vec3 *
    getMinimumTranslationVec(glm::vec3 *firstTriangle, glm::vec3 &firstTriangleNormal, glm::vec3 *secondTriangle);

    static glm::vec3 updateIfLess(glm::vec3 &first, glm::vec3 &second);

    static glm::vec3 updateIfGreater(glm::vec3 &first, glm::vec3 &second);

    static glm::vec3 *getMostOppositeVec(std::vector<glm::vec3 *> &mtvs, glm::vec3 direction);

};

#endif