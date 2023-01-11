#include "Map.h"

Map::Map(const char *mapModelPath) :
        model(Model(mapModelPath)) {
    model.buffer();
}

Model &Map::getModel() {
    return model;
}

glm::vec3 Map::getSpawnPos() {
    for (const auto &node: model.getNodes()) {
        glm::vec3 translation = glm::vec3(0);
        const glm::vec3 *pVec = getSpawnPos(node, translation);
        if (pVec != nullptr) {
            return *pVec + translation;
        }
    }
    return glm::vec3(0);
}

const glm::vec3 *Map::getSpawnPos(const Node &node, glm::vec3 &translation) {
    const Mesh *pMesh = node.getMesh();
    if (pMesh != nullptr) {
        for (const auto &primitive: pMesh->getPrimitives()) {
            if (primitive.getTexture()->name == "spawn") {
                return &node.getTransform().translation;
            }
        }
    }
    const std::vector<Node> &children = node.getChildren();
    if (!children.empty()) {
        translation = translation + node.getTransform().translation;
        for (const auto &child: children) {
            const glm::vec3 *pVec = getSpawnPos(child, translation);
            if (pVec != nullptr) {
                return pVec;
            }
        }
    }
    return nullptr;
}
