#include <iostream>
#include "Primitive.h"
#include "../intersection/IntersectionUtil.h"
#include "Texture.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/vec3.hpp"
#include "../../helpers/tri_tri_intersect.h"

Primitive::Primitive(std::vector<Vertex> &vertices,
                     std::vector<unsigned short> &indices, Texture *texture)
        : vertices(vertices), indices(indices), texture(texture) {}

void Primitive::buffer() {
    if (buffered) {
        return;
    }
    unsigned int newVAO, newVBO, newEBO;

    glGenVertexArrays(1, &newVAO);

    glGenBuffers(1, &newVBO);
    glGenBuffers(1, &newEBO);
    this->VAO = newVAO;

    glBindVertexArray(newVAO);

    glBindBuffer(GL_ARRAY_BUFFER, newVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newEBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
    buffered = true;
}

void Primitive::draw(Shader &shader) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, nullptr);
}

const Texture *Primitive::getTexture() const { return texture; }

std::vector<glm::vec3 *>
Primitive::getMinimumTranslationVec(const glm::mat4 &transMat, const Primitive &other,
                                    const glm::mat4 &otherTransMat) const {
    std::vector<glm::vec3 *> res;
    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 firstTriangle[3];
        getTriangleVertices(i, transMat, firstTriangle);
        glm::vec3 firstTriangleNormal = getTriangleNormal(i, transMat);
        for (int j = 0; j < other.indices.size(); j += 3) {
            glm::vec3 secondTriangle[3];
            other.getTriangleVertices(j, otherTransMat, secondTriangle);
            float V0[3] = {firstTriangle[0].x, firstTriangle[0].y, firstTriangle[0].z};
            float V1[3] = {firstTriangle[1].x, firstTriangle[1].y, firstTriangle[1].z};
            float V2[3] = {firstTriangle[2].x, firstTriangle[2].y, firstTriangle[2].z};
            float U0[3] = {secondTriangle[0].x, secondTriangle[0].y, secondTriangle[0].z};
            float U1[3] = {secondTriangle[1].x, secondTriangle[1].y, secondTriangle[1].z};
            float U2[3] = {secondTriangle[2].x, secondTriangle[2].y, secondTriangle[2].z};
            int isIntersecting = tri_tri_intersect(V0, V1, V2, U0, U1, U2);
            if (isIntersecting) {
                glm::vec3 *mtv = IntersectionUtil::getMinimumTranslationVec(firstTriangle, firstTriangleNormal,
                                                                            secondTriangle);
                res.push_back(mtv);
                break;
            }
        }
    }
    return res;
}

void Primitive::getTriangleVertices(int idx, const glm::mat4 &transMat, glm::vec3 *retVertices) const {
    retVertices[0] = transMat * glm::vec4(vertices[indices[idx]].position, 1.0f);
    retVertices[1] = transMat * glm::vec4(vertices[indices[idx + 1]].position, 1.0f);
    retVertices[2] = transMat * glm::vec4(vertices[indices[idx + 2]].position, 1.0f);
}

glm::vec3 Primitive::getTriangleNormal(int idx, const glm::mat4 &transMat) const {
    return glm::normalize(glm::inverseTranspose(glm::mat3(transMat)) * vertices[indices[idx]].normal);
}

const std::vector<Vertex> &Primitive::getVertices() const {
    return vertices;
}

glm::vec3 Primitive::getMin(glm::mat4 transMat) const {
    glm::vec3 min = glm::vec3(INFINITY, INFINITY, INFINITY);
    for (const auto &vertex: vertices) {
        glm::vec3 pos = transMat * glm::vec4(vertex.position, 1.0f);
        min = IntersectionUtil::updateIfLess(min, pos);
    }
    return min;
}

glm::vec3 Primitive::getMax(glm::mat4 transMat) const {
    glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    for (const auto &vertex: vertices) {
        glm::vec3 pos = transMat * glm::vec4(vertex.position, 1.0f);
        max = IntersectionUtil::updateIfGreater(max, pos);
    }
    return max;
}

AABB Primitive::calculateAABB(glm::mat4 transMat) const {
    return {getMin(transMat), getMax(transMat)};
}
