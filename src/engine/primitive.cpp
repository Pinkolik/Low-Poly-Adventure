#include "primitive.h"
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Primitive::Primitive(vector<Vertex> &vertices, vector<unsigned short> &indices,
		Texture &texture) {
	this->vertices = vertices;
	this->indices = indices;
	this->texture = texture;
}

void Primitive::buffer() {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
			vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned short), indices.data(),
			GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, texcoord));

	glBindVertexArray(0);
}

void Primitive::draw(Shader &shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}

Texture& Primitive::getTexture() {
	return texture;
}

float Primitive::findIntersectionCoefficient(glm::mat4 modelMat,
		glm::vec3 origin, glm::vec3 direction) {
	vector<float> coefficients;
	glm::mat3 normalMat = glm::inverseTranspose(modelMat);
	for (int i = 0; i < indices.size(); i += 3) {
		glm::vec3 normal = normalMat * vertices[indices[i]].normal;
		glm::vec3 a = modelMat * glm::vec4(vertices[indices[i]].position, 1);
		glm::vec3 b = modelMat
				* glm::vec4(vertices[indices[i + 1]].position, 1);
		glm::vec3 c = modelMat
				* glm::vec4(vertices[indices[i + 2]].position, 1);

		float surfaceDistanceFromOrigin = -glm::dot(normal, a);
		float dotNormalDirection = glm::dot(normal, direction);
		if (dotNormalDirection == 0.0f) {
			continue; // vector is parallel to surface
		}
		float vectorCoefficient = -(glm::dot(normal, origin)
				+ surfaceDistanceFromOrigin) / dotNormalDirection;
		if (vectorCoefficient < 0.0f) {
			continue; // vector is 0behind surface
		}
		glm::vec3 *intersection = new glm::vec3(
				origin + vectorCoefficient * direction);

		if (!Utils::isPointInsideTriangle(a, b, c, normal, *intersection)) {
			continue;
		}
		coefficients.push_back(vectorCoefficient);
	}
	return Utils::getMinFloat(coefficients);
}

vector<Vertex>& Primitive::getVertices() {
	return vertices;
}
