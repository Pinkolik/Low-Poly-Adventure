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

bool IntersectionUtil::isLess(glm::vec3 &first, glm::vec3 &second) {
    return first.x < second.x || first.y < second.y || first.z < second.z;
}

bool IntersectionUtil::isGreater(glm::vec3 &first, glm::vec3 &second) {
    return first.x > second.x || first.y > second.y || first.z > second.z;
}
