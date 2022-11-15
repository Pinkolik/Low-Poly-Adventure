#ifndef NODE_H
#define NODE_H
#include "mesh.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;

struct Node {
  Mesh mesh;
  glm::vec4 rotation = glm::vec4(1, 1, 1, 0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 translation = glm::vec3(0);
  vector<Node> children;
  bool isSpawn = false;
};

#endif