#ifndef MESH_H
#define MESH_H
#include "texture.h"
#include "vertex.h"
#include <vector>
using namespace std;
struct Mesh {
  vector<Vertex> vertices;
  Texture texture;
};
#endif