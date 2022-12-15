#ifndef MESH_H
#define MESH_H

#include "Primitive.h"
#include <vector>

class Mesh {
public:
    explicit Mesh(std::vector<Primitive> &primitives);

    std::vector<Primitive> &getPrimitives();

private:
    std::vector<Primitive> primitives;
};

#endif
