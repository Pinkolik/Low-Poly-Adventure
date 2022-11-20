#include "mesh.h"

Mesh::Mesh(vector<Primitive> &primitives) { this->primitives = primitives; }
vector<Primitive> &Mesh::getPrimitives() { return primitives; }