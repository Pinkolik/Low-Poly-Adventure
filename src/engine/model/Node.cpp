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

void Node::addChild(Node &child) {
    children.push_back(child);
}

std::vector<glm::vec3 *>
Node::getMinimumTranslationVec(const Node &other) const {
    std::vector<glm::vec3 *> res;
    if (mesh != nullptr && other.mesh != nullptr) {
        for (const Primitive &primitive: mesh->getPrimitives()) {
            for (const Primitive &otherPrimitive: other.mesh->getPrimitives()) {
                std::vector<glm::vec3 *> mtvs = primitive.getMinimumTranslationVec(otherPrimitive);
                if (!mtvs.empty()) {
                    res.insert(res.end(), mtvs.begin(), mtvs.end());
                }
            }
        }
    }

    if (!children.empty()) {
        for (const auto &child: children) {
            std::vector<glm::vec3 *> mtvs = child.getMinimumTranslationVec(other);
            if (!mtvs.empty()) {
                res.insert(res.end(), mtvs.begin(), mtvs.end());
            }
        }
    }

    if (!other.children.empty()) {
        for (const auto &child: other.children) {
            std::vector<glm::vec3 *> mtvs = getMinimumTranslationVec(child);
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

bool Node::isAABBIntersecting(const Node &other) const {
    if (mesh != nullptr && other.mesh != nullptr) {
        for (const Primitive &primitive: mesh->getPrimitives()) {
            for (const Primitive &otherPrimitive: other.mesh->getPrimitives()) {
                if (primitive.getAabb().isIntersecting(otherPrimitive.getAabb())) {
                    return true;
                }
            }
        }
    }

    if (!children.empty()) {
        for (const auto &child: children) {
            if (child.isAABBIntersecting(other)) {
                return true;
            }
        }
    }


    if (!other.children.empty()) {
        for (const auto &child: other.children) {
            if (isAABBIntersecting(child)) {
                return true;
            }
        }
    }

    return false;
}

void Node::applyTranslation(glm::mat4 transMat, bool recalculateAABBs) {
    if (mesh != nullptr) {
        mesh->applyTranslation(transMat);
        if (recalculateAABBs) {
            mesh->calculateAABBs();
        } else {
            mesh->applyTranslationToAABBs(transMat);
        }
    } else {
        for (auto &child: children) {
            child.applyTranslation(transMat, false);
        }
    }
}

void Node::init(glm::mat4 transMat) {
    transMat = transMat * ModelUtil::getTransMat(transform);
    if (mesh != nullptr) {
        mesh->applyTranslation(transMat);
        mesh->calculateAABBs();
    } else {
        for (auto &child: children) {
            child.init(transMat);
        }
    }
}

void Node::init() {
    init(glm::mat4(1));
}

void Node::translate(glm::vec3 translation) {
    transform.translation += translation;
    glm::mat4 transMat = glm::translate(glm::mat4(1), translation);
    applyTranslation(transMat, false);
}

void Node::scale(glm::vec3 scale) {
    transform.scale *= scale;
    glm::mat4 transMat = glm::scale(glm::mat4(1), scale);
    applyTranslation(transMat, false);
}
