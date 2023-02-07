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

    std::vector<glm::vec3 *> getMinimumTranslationVec(const Primitive &other) const;

    const AABB &getAabb() const;

    const Texture *getTexture() const;

    void calculateAABB();

    void applyTranslation(glm::mat4 transMat);

    void applyTranslationToAABB(glm::mat4 transMat);

private:

    bool buffered = false;
    Texture *texture = nullptr;
    unsigned int VAO = 0;

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    AABB aabb;

    void getTriangleVertices(int idx, glm::vec3 *retVertices) const;

    glm::vec3 getMin() const;

    glm::vec3 getMax() const;
};

#endif
