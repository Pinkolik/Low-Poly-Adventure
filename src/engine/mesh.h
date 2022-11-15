#ifndef MESH_H
#define MESH_H
#include "primitive.h"
#include <vector>
using namespace std;
struct Mesh {
  vector<Primitive> primitives;
};
#endif