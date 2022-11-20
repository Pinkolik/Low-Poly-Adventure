#ifndef NODE_H
#define NODE_H
#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

using namespace std;

class Node {
public:
  Node(vector<double> &rotation, vector<double> &scale,
       vector<double> &translation, Mesh &mesh);
  void buffer();
  void draw(Shader &shader);
  void addChild(Node &child);
  bool isSpawn();
  glm::vec3 &getTranslation();
  glm::vec3 *findIntersection(glm::vec3 origin, glm::vec3 direction);

private:
  glm::mat4 getModelMat();

  Mesh *mesh;
  glm::quat rotation = glm::quat(1, 0, 0, 0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 translation = glm::vec3(0);
  vector<Node> children;
  bool spawn = false;
};

#endif