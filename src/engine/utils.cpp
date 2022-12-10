#include "utils.h"
#include "glm/fwd.hpp"
#include "primitive.h"
#include "texture.h"

vector<Node> Utils::debugNodes;

float Utils::getMinFloat(vector<float> floats) {
	if (floats.empty()) {
		return MAXFLOAT;
	}
	float min = MAXFLOAT;
	for (auto f : floats) {
		if (f < min) {
			min = f;
		}
	}
	return min;
}

bool Utils::isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
		glm::vec3 normal, glm::vec3 point) {
	glm::vec3 edge0 = b - a;
	glm::vec3 edge1 = c - b;
	glm::vec3 edge2 = a - c;
	glm::vec3 c0 = point - a;
	glm::vec3 c1 = point - b;
	glm::vec3 c2 = point - c;

	return glm::dot(normal, glm::cross(edge0, c0)) > 0
			&& glm::dot(normal, glm::cross(edge1, c1)) > 0
			&& glm::dot(normal, glm::cross(edge2, c2)) > 0;
}

Node Utils::createBoxNode() {
	vector<double> rotation;
	vector<double> scale;
	vector<double> translation;

	vector<Primitive> primitives;

	float points[24] = {
	// front
			-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
			// back
			-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0 };
	vector<Vertex> vertices;
	for (unsigned int i = 0; i < 24; i += 3) {
		Vertex vertex;
		vertex.position = glm::vec3(points[i], points[i + 1], points[i + 2]);
		vertices.push_back(vertex);
	}
	vector<unsigned short> indices = { // front
			0, 1, 2, 2, 3, 0,
			// right
					1, 5, 6, 6, 2, 1,
					// back
					7, 6, 5, 5, 4, 7,
					// left
					4, 0, 3, 3, 7, 4,
					// bottom
					4, 5, 1, 1, 0, 4,
					// top
					3, 2, 6, 6, 7, 3 };
	Texture texture;

	Primitive box = Primitive(vertices, indices, texture);
	primitives.push_back(box);

	Mesh mesh = Mesh(primitives);
	Node result = Node(rotation, scale, translation, mesh);
	result.buffer();
	return result;
}
