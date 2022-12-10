#include "node.h"
#include "glm/fwd.hpp"
#include "mesh.h"
#include <glm/ext/matrix_transform.hpp>

Node::Node(std::vector<double> rotation, std::vector<double> scale,
           std::vector<double> translation, Mesh &mesh)
    : mesh(mesh) {

  if (!rotation.empty()) {
    this->rotation =
        glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
  }
  if (!scale.empty()) {
    this->scale = glm::vec3(scale[0], scale[1], scale[2]);
  }
  if (!translation.empty()) {
    this->translation =
        glm::vec3(translation[0], translation[1], translation[2]);
  }

  for (auto &primitive : mesh.getPrimitives()) {
    if (primitive.getTexture().name.compare("spawn") == 0) {
      this->spawn = true;
      break;
    }
  }
}

void Node::buffer() {
  for (Primitive &primitive : mesh.getPrimitives()) {
    primitive.buffer();
  }

  for (Node &childNode : children) {
    childNode.buffer();
  }
}

void Node::draw(Shader &shader) {
  draw(shader, glm::quat(1, 0, 0, 0), glm::vec3(1), glm::vec3(0));
}

void Node::draw(Shader &shader, glm::quat addRot, glm::vec3 addScale,
                glm::vec3 addTrans) {
  glm::mat4 modelMat = getModelMat(addRot, addScale, addTrans);
  shader.setMatrix4f("model", modelMat);
  for (Primitive &primitive : mesh.getPrimitives()) {
    primitive.draw(shader);
  }

  for (Node &childNode : children) {
    childNode.draw(shader, addRot, addScale, addTrans);
  }
}

void Node::addChild(Node &child) { children.push_back(child); }

bool Node::isSpawn() { return spawn; }

glm::vec3 Node::getTranslation() { return translation; }

glm::mat4 Node::getModelMat() {
  return getModelMat(glm::quat(1, 0, 0, 0), glm::vec3(1), glm::vec3(0));
}

glm::mat4 Node::getModelMat(glm::quat addRot, glm::vec3 addScale,
                            glm::vec3 addTrans) {
  glm::mat4 translationMat =
      glm::translate(glm::mat4(1), translation + addTrans);
  glm::mat4 rotationMat = glm::toMat4(rotation * addRot);
  glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale * addScale);
  glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
  return modelMat;
}

void Node::setTranslation(glm::vec3 translation) {
  this->translation = translation;
}

void Node::setScale(glm::vec3 scale) { this->scale = scale; }

void Node::setRotation(glm::quat rotation) { this->rotation = rotation; }

Mesh &Node::getMesh() { return mesh; }
