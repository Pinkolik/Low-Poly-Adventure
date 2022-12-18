#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "glm/fwd.hpp"
#include "../../shader/Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "../intersection/AABB.h"
#include <vector>

class Primitive {
public:
    Primitive(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices, Texture &texture);

    void buffer();

    void draw(Shader &shader);

    std::vector<glm::vec3 *> getMinimumTranslationVec(glm::mat4 &modelMat, Primitive &other, glm::mat4 &otherModelMat);

    Texture &getTexture();

    glm::vec3 getMin(glm::mat4 modelMat);

    glm::vec3 getMax(glm::mat4 modelMat);

    void calculateAABB(glm::mat4 modelMat);

    bool isAABBIntersecting(glm::vec3 &translation, Primitive &other, glm::vec3 &otherTranslation);

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    Texture texture;
    AABB *aabb = nullptr;

    std::vector<glm::vec3> getTriangleVertices(int idx, glm::mat4 &modelMat);

    glm::vec3 getTriangleNormal(int idx, glm::mat4 &modelMat);
};

#endif
