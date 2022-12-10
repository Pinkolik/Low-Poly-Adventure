#ifndef MESH_H
#define MESH_H
#include "primitive.h"
#include <vector>

class Mesh {
public:
  Mesh(std::vector<Primitive> &primitives);
  std::vector<Primitive> &getPrimitives();

private:
  std::vector<Primitive> primitives;
};
#endif
