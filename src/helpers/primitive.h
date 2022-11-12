#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "vertex.h"
#include <vector>

using namespace std;

struct Primitive {
  vector<Vertex> vertices;
  vector<unsigned short> indices;
};
#endif