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
    Primitive(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices, Texture *texture);

    void buffer();

    void draw(Shader &shader) const;

    std::vector<glm::vec3 *>
    getMinimumTranslationVec(const glm::mat4 &transMat, const Primitive &other, const glm::mat4 &otherTransMat,
                             const std::vector<AABB *> &otherAABBs) const;

    const Texture *getTexture() const;

    const std::vector<Vertex> &getVertices() const;

    AABB calculateAABB(glm::mat4 transMat) const;
private:

    bool buffered = false;
    unsigned int VAO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    Texture *texture = nullptr;

    void getTriangleVertices(int idx, const glm::mat4 &transMat, glm::vec3 *retVertices) const;

    glm::vec3 getTriangleNormal(int idx, const glm::mat4 &transMat) const;

    glm::vec3 getMin(glm::mat4 transMat) const;

    glm::vec3 getMax(glm::mat4 transMat) const;
};

#endif
