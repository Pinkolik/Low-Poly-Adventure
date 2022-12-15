#include "primitive.h"
#include "intersection/intersection_util.h"
#include "texture.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/vec3.hpp>

Primitive::Primitive(std::vector<Vertex> &vertices,
                     std::vector<unsigned short> &indices, Texture &texture)
        : vertices(vertices), indices(indices), texture(texture) {}

void Primitive::buffer() {
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    this->VAO = VAO;
    this->VBO = VBO;
    this->EBO = EBO;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short),
                 indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Primitive::draw(Shader &shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}

Texture &Primitive::getTexture() { return texture; }

std::vector<Vertex> &Primitive::getVertices() { return vertices; }

glm::vec3 *Primitive::getMinimumTranslationVec(glm::mat4 modelMat,
                                               Primitive other,
                                               glm::mat4 otherModelMat) {
    for (int i = 0; i < indices.size(); i += 3) {
        std::vector<glm::vec3> firstTriangle = getTriangleVertices(i, modelMat);
        glm::vec3 firstTriangleNormal =
                getTriangleNormal(i, modelMat);
        for (int j = 0; j < other.indices.size(); j += 3) {
            std::vector<glm::vec3> secondTriangle =
                    other.getTriangleVertices(j, otherModelMat);
            glm::vec3 secondTriangleNormal =
                    other.getTriangleNormal(j, otherModelMat);
            glm::vec3 *mtv = IntersectionUtil::getMinimumTranslationVec(
                    firstTriangle, firstTriangleNormal, secondTriangle,
                    secondTriangleNormal);
            if (mtv != NULL) {
                return mtv;
            }
        }
    }
    return NULL;
}

std::vector<glm::vec3> Primitive::getTriangleVertices(int idx,
                                                      glm::mat4 modelMat) {
    std::vector<glm::vec3> triangle;
    triangle.push_back(modelMat *
                       glm::vec4(vertices[indices[idx]].position, 1.0f));
    triangle.push_back(modelMat *
                       glm::vec4(vertices[indices[idx + 1]].position, 1.0f));
    triangle.push_back(modelMat *
                       glm::vec4(vertices[indices[idx + 2]].position, 1.0f));
    return triangle;
}

glm::vec3 Primitive::getTriangleNormal(int idx,
                                       glm::mat4 modelMat) {
    return glm::normalize(glm::inverseTranspose(glm::mat3(modelMat)) *
                          vertices[indices[idx]].normal);
}