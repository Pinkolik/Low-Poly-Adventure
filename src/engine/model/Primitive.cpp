#include "Primitive.h"
#include "../intersection/IntersectionUtil.h"
#include "Texture.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/vec3.hpp"

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
        std::vector<glm::vec3> firstTriangle = getTriangleVertices(i, transMat);
        glm::vec3 firstTriangleNormal = getTriangleNormal(i, transMat);
        for (int j = 0; j < other.indices.size(); j += 3) {
            std::vector<glm::vec3> secondTriangle = other.getTriangleVertices(j, otherTransMat);
            glm::vec3 secondTriangleNormal = other.getTriangleNormal(j, otherTransMat);
            glm::vec3 *mtv = IntersectionUtil::getMinimumTranslationVec(firstTriangle, firstTriangleNormal,
                                                                        secondTriangle, secondTriangleNormal);
            if (mtv != nullptr) {
                res.push_back(mtv);
                break;
            }
        }
    }
    return res;
}

std::vector<glm::vec3> Primitive::getTriangleVertices(int idx, const glm::mat4 &transMat) const {
    std::vector<glm::vec3> triangle;
    triangle.emplace_back(transMat * glm::vec4(vertices[indices[idx]].position, 1.0f));
    triangle.emplace_back(transMat * glm::vec4(vertices[indices[idx + 1]].position, 1.0f));
    triangle.emplace_back(transMat * glm::vec4(vertices[indices[idx + 2]].position, 1.0f));
    return triangle;
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
