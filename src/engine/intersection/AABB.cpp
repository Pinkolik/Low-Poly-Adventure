//
// Created by pinkolik on 12/18/22.
//

#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
    fillVertices();
    buffer();
}

void AABB::draw(Shader &shader) {
    shader.setMatrix4f("model", glm::mat4(1));
    shader.setBool("debug", true);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    shader.setBool("debug", false);
}

void AABB::buffer() {
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindVertexArray(0);
}

void AABB::fillVertices() {
    glm::vec3 diag = max - min;
    glm::vec3 x = glm::vec3(diag.x, 0, 0);
    glm::vec3 y = glm::vec3(0, diag.y, 0);
    glm::vec3 z = glm::vec3(0, 0, diag.z);

    //min
    vertices.push_back(min);
    vertices.push_back(min + x);
    vertices.push_back(min + y);
    vertices.push_back(min);
    vertices.push_back(min + y);
    vertices.push_back(min + z);
    vertices.push_back(min);
    vertices.push_back(min + z);
    vertices.push_back(min + x);

    vertices.push_back(min + y);
    vertices.push_back(min + y + x);
    vertices.push_back(min + x);

    vertices.push_back(min + y);
    vertices.push_back(min + y + z);
    vertices.push_back(min + z);

    vertices.push_back(min + y);
    vertices.push_back(min + y + z);
    vertices.push_back(min + y + x);

    //max
    vertices.push_back(max);
    vertices.push_back(max - x);
    vertices.push_back(max - y);
    vertices.push_back(max);
    vertices.push_back(max - y);
    vertices.push_back(max - z);
    vertices.push_back(max);
    vertices.push_back(max - z);
    vertices.push_back(max - x);

    vertices.push_back(max - y);
    vertices.push_back(max - y - x);
    vertices.push_back(max - x);

    vertices.push_back(max - y);
    vertices.push_back(max - y - z);
    vertices.push_back(max - z);

    vertices.push_back(max - y);
    vertices.push_back(max - y - z);
    vertices.push_back(max - y - x);

}

bool AABB::isIntersecting(glm::vec3 translation, AABB *other, glm::vec3 otherTranslation) {
    glm::vec3 firstMin = translation + min;
    glm::vec3 firstMax = translation + max;
    glm::vec3 secondMin = otherTranslation + other->min;
    glm::vec3 secondMax = otherTranslation + other->max;
    return firstMin.x <= secondMax.x && firstMax.x >= secondMin.x &&
           firstMin.y <= secondMax.y && firstMax.y >= secondMin.y &&
           firstMin.z <= secondMax.z && firstMax.z >= secondMin.z;
}
