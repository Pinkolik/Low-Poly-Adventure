#include "IntersectionUtil.h"
#include "glm/geometric.hpp"
#include "Projection.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

IntersectionResult *
IntersectionUtil::getMinimumTranslationVec(std::vector<glm::vec3> &firstTriangle, glm::vec3 &firstTriangleNormal,
                                           std::vector<glm::vec3> &secondTriangle, glm::vec3 &secondTriangleNormal) {
    std::vector<glm::vec3> separatingAxes;
    std::vector<glm::vec3> firstAxes = getSeparatingAxes(firstTriangle, firstTriangleNormal);
    std::vector<glm::vec3> secondAxes = getSeparatingAxes(secondTriangle, secondTriangleNormal);
    separatingAxes.push_back(firstTriangleNormal);
    separatingAxes.push_back(secondTriangleNormal);
    separatingAxes.insert(separatingAxes.end(), firstAxes.begin(), firstAxes.end());
    separatingAxes.insert(separatingAxes.end(), secondAxes.begin(), secondAxes.end());
    float trans = INFINITY;
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
    glm::vec3 mtv = glm::vec3(mtvAxis * trans * 1.5f);
    return new IntersectionResult{firstTriangle, firstTriangleNormal, secondTriangle, secondTriangleNormal, mtv};
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

glm::vec3 *IntersectionUtil::getMostOppositeVec(std::vector<IntersectionResult *> &intersections, glm::vec3 direction) {
    glm::vec3 *res = nullptr;
    float minDot = INFINITY;
    glm::vec3 minNormMtv;
    if (!intersections.empty()) {
        glm::vec3 normDir = glm::normalize(direction);
        std::cout << "Norm Dir: " << normDir.x << ", " << normDir.y << ", " << normDir.z << std::endl;
        for (auto &intersection: intersections) {
            glm::vec3 normMtv = glm::normalize(intersection->mtv);
            float dot = glm::dot(normDir, normMtv);
            if (dot < minDot) {
                minDot = dot;
                res = &(intersection->mtv);
                minNormMtv = normMtv;
            }
        }

    }
    if (res != nullptr) {
        std::cout << "Min Norm MTV: " << minNormMtv.x << ", " << minNormMtv.y << ", " << minNormMtv.z << std::endl;
    }
    return res;
}

std::vector<IntersectionResult *>
IntersectionUtil::recalculateIntersections(std::vector<IntersectionResult *> &intersections,
                                           glm::vec3 *translationForSecond) {
    if (translationForSecond == nullptr) {
        return intersections;
    }
    std::vector<IntersectionResult *> result;
    for (auto &intersection: intersections) {
        std::vector<glm::vec3> secondTriangle;
        glm::vec3 &vec = *translationForSecond;
        for (auto &vertex: intersection->secondTriangle) {
            secondTriangle.push_back(vertex + vec);
        }
        IntersectionResult *newIntersection = IntersectionUtil::getMinimumTranslationVec(intersection->firstTriangle,
                                                                                         intersection->firstTriangleNormal,
                                                                                         secondTriangle,
                                                                                         intersection->secondTriangleNormal);
        if (newIntersection != nullptr) {
            result.push_back(newIntersection);
        }
        delete intersection;
    }
    return result;
}

glm::vec3 *IntersectionUtil::getSmallestVec(std::vector<IntersectionResult *> &intersections) {
    glm::vec3 *res = nullptr;
    float minLen = INFINITY;
    if (!intersections.empty()) {
        for (auto &intersection: intersections) {
            float len = glm::length(intersection->mtv);
            if (len < minLen) {
                minLen = len;
                res = &(intersection->mtv);
            }
        }

    }
    return res;
}
