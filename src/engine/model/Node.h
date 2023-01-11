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

    void buffer();

    void draw(Shader &shader, glm::mat4 transMat);

    void addChild(Node &child);

    void calculateAABBs(glm::mat4 transMat);

    bool isAABBIntersecting(glm::mat4 transMat, const Node &other, glm::mat4 otherTransMat) const;

    const Mesh *getMesh() const;

    const TransformationStruct &getTransform() const;

    const std::vector<Node> &getChildren() const;

    std::vector<glm::vec3 *>
    getMinimumTranslationVec(glm::mat4 transMat, const Node &other, glm::mat4 otherTransMat) const;

private:

    Mesh *mesh = nullptr;
    std::vector<AABB *> aabbs;
    TransformationStruct transform;
    std::vector<Node> children;

};

#endif
