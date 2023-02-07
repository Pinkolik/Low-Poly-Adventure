#include <iostream>
#include "Primitive.h"
#include "../intersection/IntersectionUtil.h"
#include "Texture.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/vec3.hpp"
#include "../../helpers/tri_tri_intersect.h"

Primitive::Primitive(std::vector<Vertex> &vertices,
                     std::vector<unsigned short> &indices, Texture *texture)
        : vertices(vertices), indices(indices), texture(texture), aabb(glm::vec3(0), glm::vec3()) {}

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
Primitive::getMinimumTranslationVec(const Primitive &other) const {
    glm::vec3 firstVertices[indices.size()];
    glm::vec3 firstNormals[indices.size() / 3];
    std::vector<glm::vec3 *> res;
    const AABB &otherAABB = other.aabb;
    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 *firstTriangle = firstVertices + i;
        getTriangleVertices(i, firstTriangle);
        firstNormals[i / 3] = vertices[indices[i]].normal;
        glm::vec3 firstTriangleNormal = firstNormals[i / 3];
        bool isInside = otherAABB.isInside(firstTriangle);
        if (isInside) {
            glm::vec3 *mtv = IntersectionUtil::getMinimumTranslationVec(firstTriangle, firstTriangleNormal, otherAABB);
            res.push_back(mtv);
        }
    }
    if (!res.empty()) {
        return res;
    }
    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 *firstTriangle = firstVertices + i;
        glm::vec3 firstTriangleNormal = firstNormals[i / 3];
        for (int j = 0; j < other.indices.size(); j += 3) {
            glm::vec3 secondTriangle[3];
            other.getTriangleVertices(j, secondTriangle);
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

void Primitive::getTriangleVertices(int idx, glm::vec3 *retVertices) const {
    retVertices[0] = vertices[indices[idx]].position;
    retVertices[1] = vertices[indices[idx + 1]].position;
    retVertices[2] = vertices[indices[idx + 2]].position;
}

glm::vec3 Primitive::getMin() const {
    glm::vec3 min = glm::vec3(INFINITY, INFINITY, INFINITY);
    for (const auto &vertex: vertices) {
        IntersectionUtil::updateIfLess(min, vertex.position);
    }
    return min;
}

glm::vec3 Primitive::getMax() const {
    glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    for (const auto &vertex: vertices) {
        IntersectionUtil::updateIfGreater(max, vertex.position);
    }
    return max;
}

void Primitive::applyTranslation(glm::mat4 transMat) {
    const glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(transMat));
    for (auto &vertex: vertices) {
        vertex.position = transMat * glm::vec4(vertex.position, 1.0f);
        vertex.normal = glm::normalize(normalMatrix * vertex.normal);
    }
}

void Primitive::calculateAABB() {
    aabb = {getMin(), getMax()};
}

const AABB &Primitive::getAabb() const {
    return aabb;
}

void Primitive::applyTranslationToAABB(glm::mat4 transMat) {
    aabb.applyTranslation(transMat);
}
