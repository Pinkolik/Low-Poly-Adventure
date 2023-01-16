#include <iostream>
#include "Node.h"
#include "Mesh.h"
#include "TransformationStruct.h"
#include "glm/gtx/quaternion.hpp"
#include "../intersection/IntersectionUtil.h"
#include "ModelUtil.h"

Node::Node(std::vector<double> rotation, std::vector<double> scale,
           std::vector<double> translation, Mesh *mesh)
        : mesh(mesh) {

    if (!rotation.empty()) {
        transform.rotation =
                glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
    }
    if (!scale.empty()) {
        transform.scale = glm::vec3(scale[0], scale[1], scale[2]);
    }
    if (!translation.empty()) {
        transform.translation =
                glm::vec3(translation[0], translation[1], translation[2]);
    }
}

void Node::buffer() {
    if (mesh != nullptr) {
        mesh->buffer();
    }

    for (Node &childNode: children) {
        childNode.buffer();
    }
}

void Node::draw(Shader &shader, glm::mat4 transMat) {
    transMat = transMat * ModelUtil::getTransMat(transform);
    if (mesh != nullptr) {
        shader.setMatrix4f("model", transMat);
        for (const Primitive &primitive: mesh->getPrimitives()) {
            primitive.draw(shader);
        }
    }

    for (Node &childNode: children) {
        childNode.draw(shader, transMat);
    }
}

void Node::addChild(Node &child) { children.push_back(child); }

std::vector<glm::vec3 *>
Node::getMinimumTranslationVec(glm::mat4 transMat, const Node &other, glm::mat4 otherTransMat) const {
    std::vector<glm::vec3 *> res;
    if (mesh != nullptr && other.mesh != nullptr) {
        transMat = transMat * ModelUtil::getTransMat(transform);
        otherTransMat = otherTransMat * ModelUtil::getTransMat(other.transform);
        for (const Primitive &primitive: mesh->getPrimitives()) {
            for (const Primitive &otherPrimitive: other.mesh->getPrimitives()) {
                std::vector<AABB *> otherAABBS = other.mesh->calculateAABBs(otherTransMat);
                std::vector<glm::vec3 *> mtvs = primitive.getMinimumTranslationVec(transMat,
                                                                                   otherPrimitive,
                                                                                   otherTransMat,
                                                                                   otherAABBS);
                if (!mtvs.empty()) {
                    res.insert(res.end(), mtvs.begin(), mtvs.end());
                }
            }
        }
    }

    if (!children.empty()) {
        transMat = transMat * ModelUtil::getTransMat(transform);
        for (const auto &child: children) {
            std::vector<glm::vec3 *> mtvs = child.getMinimumTranslationVec(transMat, other, otherTransMat);
            if (!mtvs.empty()) {
                res.insert(res.end(), mtvs.begin(), mtvs.end());
            }
        }
    }

    if (!other.children.empty()) {
        otherTransMat = otherTransMat * ModelUtil::getTransMat(other.transform);
        for (const auto &child: other.children) {
            std::vector<glm::vec3 *> mtvs = getMinimumTranslationVec(transMat, child, otherTransMat);
            if (!mtvs.empty()) {
                res.insert(res.end(), mtvs.begin(), mtvs.end());
            }
        }
    }
    return res;
}

const Mesh *Node::getMesh() const {
    return mesh;
}

const TransformationStruct &Node::getTransform() const {
    return transform;
}

const std::vector<Node> &Node::getChildren() const {
    return children;
}

void Node::calculateAABBs(glm::mat4 transMat) {
    transMat = transMat * ModelUtil::getTransMat(transform);
    if (mesh != nullptr) {
        std::vector<AABB *> meshAABBs = mesh->calculateAABBs(transMat);
        aabbs.insert(aabbs.end(), meshAABBs.begin(), meshAABBs.end());
    }
    for (auto &child: children) {
        child.calculateAABBs(transMat);
    }
}

bool Node::isAABBIntersecting(glm::mat4 transMat, const Node &other, glm::mat4 otherTransMat) const {
    if (!aabbs.empty() && !other.aabbs.empty()) {
        for (const auto &aabb: aabbs) {
            for (const auto &otherAABB: other.aabbs) {
                if (aabb->isIntersecting(transMat, otherAABB, otherTransMat)) {
                    return true;
                }
            }
        }
    }

    for (const auto &child: children) {
        if (child.isAABBIntersecting(transMat, other, otherTransMat)) {
            return true;
        }
    }


    for (const auto &child: other.children) {
        if (isAABBIntersecting(transMat, child, otherTransMat)) {
            return true;
        }
    }

    return false;
}
