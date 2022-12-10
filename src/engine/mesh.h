#ifndef MESH_H
#define MESH_H
#include "primitive.h"
#include <vector>
using namespace std;
class Mesh {
public:
	Mesh(vector<Primitive> &primitives);
	vector<Primitive>& getPrimitives();

private:
	vector<Primitive> primitives;
};
#endif
