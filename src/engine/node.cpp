#include "node.h"
#include "glm/fwd.hpp"
#include "mesh.h"
#include "position_struct.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Node::Node(std::vector<double> rotation, std::vector<double> scale,
           std::vector<double> translation, Mesh &mesh)
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

    for (auto &primitive: mesh.getPrimitives()) {
        if (primitive.getTexture().name.compare("spawn") == 0) {
            spawn = true;
            break;
        }
    }
}

void Node::buffer() {
    for (Primitive &primitive: mesh.getPrimitives()) {
        primitive.buffer();
    }

    for (Node &childNode: children) {
        childNode.buffer();
    }
}

void Node::draw(Shader &shader) {
    PositionStruct modelPos;
    draw(shader, modelPos);
}

void Node::draw(Shader &shader, PositionStruct modelPos) {
    glm::mat4 modelMat = getModelMat(modelPos);
    shader.setMatrix4f("model", modelMat);
    shader.setBool("intersectionDetected", intersectionDetected);
    for (Primitive &primitive: mesh.getPrimitives()) {
        primitive.draw(shader);
    }

    for (Node &childNode: children) {
        childNode.draw(shader, modelPos);
    }
    intersectionDetected = false;
}

void Node::addChild(Node &child) { children.push_back(child); }

bool Node::isSpawn() { return spawn; }

glm::vec3 Node::getTranslation() { return position.translation; }

glm::mat4 Node::getModelMat() {
    PositionStruct modelPos;
    return getModelMat(modelPos);
}

glm::mat4 Node::getModelMat(PositionStruct modelPos) {
    glm::mat4 translationMat =
            glm::translate(glm::mat4(1), position.translation + modelPos.translation);
    glm::mat4 rotationMat = glm::toMat4(position.rotation * modelPos.rotation);
    glm::mat4 scaleMat =
            glm::scale(glm::mat4(1), position.scale * modelPos.scale);
    glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
    return modelMat;
}

void Node::setTranslation(glm::vec3 translation) {
    position.translation = translation;
}

void Node::setScale(glm::vec3 scale) { position.scale = scale; }

void Node::setRotation(glm::quat rotation) { position.rotation = rotation; }

Mesh &Node::getMesh() { return mesh; }

std::vector<glm::vec3 *> Node::getMinimumTranslationVec(PositionStruct modelPos, Node &other,
                                                        PositionStruct otherModelPos) {
    std::vector<glm::vec3 *> res;
    glm::mat4 modelMat = getModelMat(modelPos);
    glm::mat4 otherModelMat = other.getModelMat(otherModelPos);
    for (Primitive &primitive: mesh.getPrimitives()) {
        for (Primitive &otherPrimitive: other.mesh.getPrimitives()) {
            std::vector<glm::vec3 *> mtvs = primitive.getMinimumTranslationVec(
                    modelMat, otherPrimitive, otherModelMat);
            if (!mtvs.empty()) {
                intersectionDetected = true;
                res.insert(res.end(), mtvs.begin(), mtvs.end());
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