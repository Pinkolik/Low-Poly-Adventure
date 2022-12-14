#ifndef NODE_H
#define NODE_H

#include "mesh.h"
#include "position_struct.h"
#include "shader.h"
#include <glm/vec3.hpp>
#include <vector>

class Node {
public:
    Node(std::vector<double> rotation, std::vector<double> scale,
         std::vector<double> translation, Mesh &mesh);

    void buffer();

    void draw(Shader &shader);

    void draw(Shader &shader, PositionStruct modelPos);

    void addChild(Node &child);

    bool isSpawn();

    void setTranslation(glm::vec3 translation);

    void setScale(glm::vec3 scale);

    void setRotation(glm::quat rotation);

    glm::vec3 getTranslation();

    Mesh &getMesh();

    glm::vec3 *getMinimumTranslationVec(PositionStruct modelPos, Node &other,
                                        PositionStruct otherModelPos);

private:
    Mesh mesh;

    PositionStruct position;

    std::vector<Node> children;
    bool spawn = false;
    bool intersectionDetected = false;

    glm::mat4 getModelMat();

    glm::mat4 getModelMat(PositionStruct modelPos);
};

#endif
