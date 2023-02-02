#ifndef NODE_H
#define NODE_H

#include "Mesh.h"
#include "TransformationStruct.h"
#include "../../shader/Shader.h"
#include "glm/vec3.hpp"
#include "../intersection/AABB.h"
#include <vector>

class Node {
public:
    Node(std::vector<double> rotation, std::vector<double> scale,
         std::vector<double> translation, Mesh *mesh);

    void draw(Shader &shader, glm::mat4 transMat = glm::mat4(1));

    void addChild(Node &child);

    bool isAABBIntersecting(const Node &other) const;

    const Mesh *getMesh() const;

    const TransformationStruct &getTransform() const;

    const std::vector<Node> &getChildren() const;

    std::vector<glm::vec3 *> getMinimumTranslationVec(const Node &other) const;

    void translate(glm::vec3 translation);

    void scale(glm::vec3 scale);

    void init();

private:

    Mesh *mesh = nullptr;

    TransformationStruct transform;

    std::vector<Node> children;

    void init(glm::mat4 transMat);

    void applyTranslation(glm::mat4 transMat, bool recalculateAABBs);

};

#endif
