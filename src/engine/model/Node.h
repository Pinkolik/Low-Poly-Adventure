#ifndef NODE_H
#define NODE_H

#include "Mesh.h"
#include "PositionStruct.h"
#include "../../shader/Shader.h"
#include "glm/vec3.hpp"
#include <vector>

class Node {
public:
    Node(std::vector<double> rotation, std::vector<double> scale,
         std::vector<double> translation, Mesh &mesh);

    void buffer();

    void draw(Shader &shader, PositionStruct modelPos);

    void addChild(Node &child);

    bool isSpawn() const;

    glm::vec3 getTranslation() const;

    std::vector<glm::vec3 *> getMinimumTranslationVec(PositionStruct modelPos, Node &other,
                                                      PositionStruct otherModelPos);

private:
    Mesh mesh;

    PositionStruct position;

    std::vector<Node> children;
    bool spawn = false;
    bool intersectionDetected = false;

    glm::mat4 getModelMat(PositionStruct modelPos) const;
};

#endif
