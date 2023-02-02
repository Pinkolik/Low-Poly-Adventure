#include "Mesh.h"

Mesh::Mesh(int id, std::vector<Primitive> &primitives) : id(id), primitives(primitives) {
    buffer();
}

const std::vector<Primitive> &Mesh::getPrimitives() const { return primitives; }

int Mesh::getId() const {
    return id;
}

void Mesh::buffer() {
    for (auto &primitive: primitives) {
        primitive.buffer();
    }
}

void Mesh::applyTranslation(glm::mat4 transMat) {
    for (auto &primitive: primitives) {
        primitive.applyTranslation(transMat);
    }
}

void Mesh::applyTranslationToAABBs(glm::mat4 transMat) {
    for (auto &primitive: primitives) {
        primitive.applyTranslationToAABB(transMat);
    }
}

void Mesh::calculateAABBs() {
    for (auto &primitive: primitives) {
        primitive.calculateAABB();
    }
}
