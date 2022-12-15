#include "Mesh.h"

Mesh::Mesh(std::vector<Primitive> &primitives) : primitives(primitives) {}

std::vector<Primitive> &Mesh::getPrimitives() { return primitives; }
