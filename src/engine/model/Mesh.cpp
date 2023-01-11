#include "Mesh.h"

Mesh::Mesh(int id, std::vector<Primitive> &primitives) : id(id), primitives(primitives) {}

const std::vector<Primitive> &Mesh::getPrimitives() const { return primitives; }

int Mesh::getId() const {
    return id;
}

void Mesh::buffer() {
    for (auto &primitive: primitives) {
        primitive.buffer();
    }
}

std::vector<AABB *> Mesh::calculateAABBs(glm::mat4 transMat) {
    std::vector<AABB *> res;
    for (const auto &primitive: primitives) {
        res.push_back(new AABB(primitive.calculateAABB(transMat)));
    }
    return res;
}
