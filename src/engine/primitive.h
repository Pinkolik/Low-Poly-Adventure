#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "texture.h"
#include "vertex.h"
#include <vector>

using namespace std;

struct Primitive {
  unsigned int VAO, VBO, EBO;
  vector<Vertex> vertices;
  vector<unsigned short> indices;
  Texture texture;
};
#endif