//
// Created by pinkolik on 12/18/22.
//

#ifndef GAME_AABB_H
#define GAME_AABB_H


#include <vector>
#include "glm/vec3.hpp"
#include "../../shader/Shader.h"

class AABB {
public:
    AABB(glm::vec3 min, glm::vec3 max);

    void buffer();

    void draw(Shader &shader);

    bool isIntersecting(glm::mat4 modelMat, AABB *other, glm::mat4 otherModelMat);

private:
    unsigned int VAO;
    glm::vec3 min, max;
    std::vector<glm::vec3> vertices;

    void fillVertices();
};


#endif //GAME_AABB_H
