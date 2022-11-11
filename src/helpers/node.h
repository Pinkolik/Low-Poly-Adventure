#ifndef NODE_H
#define NODE_H
#include "mesh.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;

struct Node {
  vector<Mesh> meshes;
  glm::vec4 rotation;
  glm::vec3 scale;
  glm::vec3 translation;
};

#endif