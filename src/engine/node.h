#ifndef NODE_H
#define NODE_H
#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

class Node {
public:
  Node(std::vector<double> rotation, std::vector<double> scale,
       std::vector<double> translation, Mesh &mesh);

  void buffer();
  void draw(Shader &shader);
  void draw(Shader &shader, glm::quat addRot, glm::vec3 addScale,
            glm::vec3 addTrans);
  void addChild(Node &child);
  bool isSpawn();

  void setTranslation(glm::vec3 translation);
  void setScale(glm::vec3 scale);
  void setRotation(glm::quat rotation);

  glm::vec3 getTranslation();
  Mesh &getMesh();

private:
  Mesh mesh;

  glm::quat rotation = glm::quat(1, 0, 0, 0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 translation = glm::vec3(0);

  std::vector<Node> children;
  bool spawn = false;

  glm::mat4 getModelMat();
  glm::mat4 getModelMat(glm::quat addRot, glm::vec3 addScale,
                        glm::vec3 addTrans);
};

#endif
