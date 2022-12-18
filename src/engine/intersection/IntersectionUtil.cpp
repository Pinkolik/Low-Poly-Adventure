#include "IntersectionUtil.h"
#include "glm/geometric.hpp"
#include "Projection.h"
#include <glm/glm.hpp>
#include <vector>

glm::vec3 *
IntersectionUtil::getMinimumTranslationVec(std::vector<glm::vec3> &firstTriangle, glm::vec3 &firstTriangleNormal,
                                           std::vector<glm::vec3> &secondTriangle, glm::vec3 &secondTriangleNormal) {
    std::vector<glm::vec3> separatingAxes;
    std::vector<glm::vec3> firstAxes = getSeparatingAxes(firstTriangle, firstTriangleNormal);
    std::vector<glm::vec3> secondAxes = getSeparatingAxes(secondTriangle, secondTriangleNormal);
    separatingAxes.push_back(firstTriangleNormal);
    separatingAxes.push_back(secondTriangleNormal);
    separatingAxes.insert(separatingAxes.end(), firstAxes.begin(), firstAxes.end());
    separatingAxes.insert(separatingAxes.end(), secondAxes.begin(), secondAxes.end());
    float trans;
    glm::vec3 mtvAxis;
    for (auto &axis: separatingAxes) {
        Projection firstProj = Projection(axis, firstTriangle);
        Projection secondProj = Projection(axis, secondTriangle);
        if (!firstProj.isIntersecting(secondProj)) {
            return nullptr;
        }
        float intersectionLen = firstProj.findIntersectionLength(secondProj);
        if (axis == firstTriangleNormal) {
            mtvAxis = axis;
            trans = intersectionLen;
        }
    }
    return new glm::vec3(mtvAxis * trans * 1.5f);
}

std::vector<glm::vec3> IntersectionUtil::getSeparatingAxes(std::vector<glm::vec3> &triangle, glm::vec3 &normal) {
    glm::vec3 ab, bc, ca;
    ab = triangle[1] - triangle[0];
    bc = triangle[2] - triangle[1];
    ca = triangle[0] - triangle[2];

    std::vector<glm::vec3> res;
    res.push_back(glm::normalize(glm::cross(normal, ab)));
    res.push_back(glm::normalize(glm::cross(normal, bc)));
    res.push_back(glm::normalize(glm::cross(normal, ca)));
    return res;
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

glm::vec3 *IntersectionUtil::getMostOppositeVec(std::vector<glm::vec3 *> mtvs, glm::vec3 direction) {
    glm::vec3 *res = nullptr;
    float minDot = INFINITY;
    if (!mtvs.empty()) {
        glm::vec3 normDir = glm::normalize(direction);
        for (const auto &mtv: mtvs) {
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
