#include <iostream>
#include "Node.h"
#include "Mesh.h"
#include "PositionStruct.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "../intersection/IntersectionUtil.h"

Node::Node(std::vector<double> rotation, std::vector<double> scale,
           std::vector<double> translation, Mesh *mesh)
        : mesh(mesh) {

    if (!rotation.empty()) {
        position.rotation =
                glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
    }
    if (!scale.empty()) {
        position.scale = glm::vec3(scale[0], scale[1], scale[2]);
    }
    if (!translation.empty()) {
        position.translation =
                glm::vec3(translation[0], translation[1], translation[2]);
    }

    if (this->mesh != nullptr) {
        glm::mat4 modelMat = getModelMat(PositionStruct());
        for (auto &primitive: this->mesh->getPrimitives()) {
            if (primitive.getTexture().name == "spawn") {
                spawn = true;
            }
            primitive.calculateAABB(modelMat);
        }
    }
}

void Node::buffer() {
    if (mesh != nullptr) {
        for (Primitive &primitive: mesh->getPrimitives()) {
            primitive.buffer();
        }
    }

    for (Node &childNode: children) {
        childNode.buffer();
    }
}

void Node::draw(Shader &shader, PositionStruct modelPos) {
    glm::mat4 modelMat = getModelMat(modelPos);
    shader.setMatrix4f("model", modelMat);
    if (mesh != nullptr) {
        for (Primitive &primitive: mesh->getPrimitives()) {
            primitive.draw(shader);
        }
    }

    for (Node &childNode: children) {
        childNode.draw(shader, modelPos);
    }
}

void Node::addChild(Node &child) { children.push_back(child); }

bool Node::isSpawn() const { return spawn; }

glm::vec3 Node::getTranslation() const { return position.translation; }

glm::mat4 Node::getModelMat(PositionStruct modelPos) const {
    glm::mat4 translationMat =
            glm::translate(glm::mat4(1), position.translation + modelPos.translation);
    glm::mat4 rotationMat = glm::toMat4(position.rotation * modelPos.rotation);
    glm::mat4 scaleMat =
            glm::scale(glm::mat4(1), position.scale * modelPos.scale);
    glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
    return modelMat;
}

std::vector<glm::vec3 *> Node::getMinimumTranslationVec(PositionStruct &modelPos, Node &other,
                                                        PositionStruct &otherModelPos) {
    std::vector<glm::vec3 *> res;
    if (mesh != nullptr) {
        glm::mat4 modelMat = getModelMat(modelPos);
        glm::mat4 otherModelMat = other.getModelMat(otherModelPos);
        for (Primitive &primitive: mesh->getPrimitives()) {
            for (Primitive &otherPrimitive: other.mesh->getPrimitives()) {
                bool isAabbIntersecting = primitive.isAABBIntersecting(modelPos.translation, otherPrimitive,
                                                                       otherModelPos.translation);
                if (!isAabbIntersecting) {
                    continue;
                }
                std::vector<glm::vec3 *> mtvs = primitive.getMinimumTranslationVec(modelMat,
                                                                                   otherPrimitive,
                                                                                   otherModelMat);
                if (!mtvs.empty()) {
                    res.insert(res.end(), mtvs.begin(), mtvs.end());
                }
            }
        }
    }

    for (Node &childNode: children) {
        std::vector<glm::vec3 *> mtvs =
                childNode.getMinimumTranslationVec(modelPos, other, otherModelPos);
        if (!mtvs.empty()) {
            res.insert(res.end(), mtvs.begin(), mtvs.end());
        }
    }
    return res;
}

