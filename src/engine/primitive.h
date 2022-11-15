#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "texture.h"
#include "vertex.h"
#include <vector>

using namespace std;

struct Primitive {
  unsigned VAO, VBO, EBO;
  vector<Vertex> vertices;
  vector<unsigned short> indices;
  Texture texture;
  glm::vec3 debugColor = glm::vec3(0);
};
#endif