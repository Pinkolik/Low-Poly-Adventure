#include "node.h"
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>

Node::Node(vector<double> &rotation, vector<double> &scale,
		vector<double> &translation, Mesh &mesh) {

	if (!rotation.empty()) {
		this->rotation = glm::quat(rotation[3], rotation[0], rotation[1],
				rotation[2]);
	}
	if (!scale.empty()) {
		this->scale = glm::vec3(scale[0], scale[1], scale[2]);
	}
	if (!translation.empty()) {
		this->translation = glm::vec3(translation[0], translation[1],
				translation[2]);
	}

	this->mesh = new Mesh(mesh);
	for (auto &primitive : mesh.getPrimitives()) {
		if (primitive.getTexture().name.compare("spawn") == 0) {
			this->spawn = true;
			break;
		}
	}
}

void Node::buffer() {
	for (Primitive &primitive : mesh->getPrimitives()) {
		primitive.buffer();
	}

	for (Node &childNode : children) {
		childNode.buffer();
	}
}

void Node::draw(Shader &shader) {
	glm::mat4 modelMat = getModelMat();
	shader.setMatrix4f("model", modelMat);
	for (Primitive &primitive : mesh->getPrimitives()) {
		primitive.draw(shader);
	}

	for (Node &childNode : children) {
		childNode.draw(shader);
	}
}

void Node::addChild(Node &child) {
	children.push_back(child);
}

bool Node::isSpawn() {
	return spawn;
}

glm::vec3& Node::getTranslation() {
	return translation;
}

float Node::findIntersectionCoefficient(glm::vec3 origin, glm::vec3 direction) {
	vector<float> coefficients;
	float minDistance = MAXFLOAT;
	glm::mat4 modelMat = getModelMat();
	for (Primitive &primitive : mesh->getPrimitives()) {
		float coefficient = primitive.findIntersectionCoefficient(modelMat,
				origin, direction);
		if (coefficient == MAXFLOAT) {
			continue;
		}
		coefficients.push_back(coefficient);
	}

	for (Node &childNode : children) {
		float coefficient = childNode.findIntersectionCoefficient(origin,
				direction);
		if (coefficient == MAXFLOAT) {
			continue;
		}
		coefficients.push_back(coefficient);
	}

	return Utils::getMinFloat(coefficients);
}

float Node::findIntersectionCoefficient(Node &node, glm::vec3 direction) {
	vector<float> coefficients;
	float minDistance = MAXFLOAT;
	glm::mat4 modelMat = getModelMat();
	for (Primitive &primitive : mesh->getPrimitives()) {
		for (Primitive &anotherPrimitive : node.mesh->getPrimitives()) {
			for (Vertex &vertex : anotherPrimitive.getVertices()) {
				glm::vec3 origin = node.getModelMat()
						* glm::vec4(vertex.position, 1.0f);
				float coefficient = primitive.findIntersectionCoefficient(
						modelMat, origin, direction);
				if (coefficient == MAXFLOAT) {
					continue;
				}
				coefficients.push_back(coefficient);
			}
		}
	}

	for (Node &childNode : children) {
		float coefficient = childNode.findIntersectionCoefficient(node,
				direction);
		if (coefficient == MAXFLOAT) {
			continue;
		}
		coefficients.push_back(coefficient);
	}

	return Utils::getMinFloat(coefficients);
}

glm::mat4 Node::getModelMat() {
	glm::mat4 translationMat = glm::translate(glm::mat4(1), translation);
	glm::mat4 rotationMat = glm::toMat4(rotation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);
	glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
	return modelMat;
}

void Node::setTranslation(glm::vec3 translation) {
	this->translation = translation;
}

void Node::setScale(glm::vec3 scale) {
	this->scale = scale;
}

void Node::setRotation(glm::quat rotation) {
	this->rotation = rotation;
}

Mesh* Node::getMesh() {
	return mesh;
}
