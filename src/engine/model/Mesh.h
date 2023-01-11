#ifndef MESH_H
#define MESH_H

#include "Primitive.h"
#include <vector>

class Mesh {
public:
    Mesh(int id, std::vector<Primitive> &primitives);

    const std::vector<Primitive> &getPrimitives() const;

    int getId() const;

    void buffer();

    std::vector<AABB *> calculateAABBs(glm::mat4 transMat);

private:
    const int id;
    std::vector<Primitive> primitives;
};

#endif
