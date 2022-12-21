//
// Created by pinkolik on 12/18/22.
//

#ifndef GAME_INTERSECTIONRESULT_H
#define GAME_INTERSECTIONRESULT_H

#include <vector>
#include "glm/vec3.hpp"

struct IntersectionResult {
    std::vector<glm::vec3> firstTriangle;
    glm::vec3 firstTriangleNormal;
    std::vector<glm::vec3> secondTriangle;
    glm::vec3 secondTriangleNormal;
    glm::vec3 mtv;
};
#endif //GAME_INTERSECTIONRESULT_H
