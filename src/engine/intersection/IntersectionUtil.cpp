#include "IntersectionUtil.h"
#include "glm/geometric.hpp"
#include "Projection.h"
#include <glm/glm.hpp>
#include <vector>

glm::vec3 *
IntersectionUtil::getMinimumTranslationVec(glm::vec3 *firstTriangle, glm::vec3 &firstTriangleNormal,
                                           glm::vec3 *secondTriangle) {
    Projection firstProj = Projection(firstTriangleNormal, firstTriangle);
    Projection secondProj = Projection(firstTriangleNormal, secondTriangle);
    float intersectionLen = firstProj.findIntersectionLength(secondProj);
    return new glm::vec3(firstTriangleNormal * intersectionLen * 1.5f);
}

glm::vec3 IntersectionUtil::updateIfLess(glm::vec3 &first, glm::vec3 &second) {
    glm::vec3 res = first;
    if (second.x < first.x) {
        res.x = second.x;
    }
    if (second.y < first.y) {
        res.y = second.y;
    }
    if (second.z < first.z) {
        res.z = second.z;
    }
    return res;
}

glm::vec3 IntersectionUtil::updateIfGreater(glm::vec3 &first, glm::vec3 &second) {
    glm::vec3 res = first;
    if (second.x > first.x) {
        res.x = second.x;
    }
    if (second.y > first.y) {
        res.y = second.y;
    }
    if (second.z > first.z) {
        res.z = second.z;
    }
    return res;
}

glm::vec3 *IntersectionUtil::getMostOppositeVec(std::vector<glm::vec3 *> &mtvs, glm::vec3 direction) {
    glm::vec3 *res = nullptr;
    float minDot = INFINITY;
    if (!mtvs.empty()) {
        glm::vec3 normDir = glm::normalize(direction);
        for (auto &mtv: mtvs) {
            glm::vec3 normMtv = glm::normalize(*mtv);
            float dot = glm::dot(normDir, normMtv);
            if (dot < minDot) {
                minDot = dot;
                res = mtv;
            }
        }
    }
    return res;
}