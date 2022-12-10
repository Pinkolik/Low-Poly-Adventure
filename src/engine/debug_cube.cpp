#include "debug_cube.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdexcept>

bool DebugCube::initialized = false;
unsigned int DebugCube::VAO = 0;
unsigned int DebugCube::VBO = 0;
unsigned int DebugCube::EBO = 0;
std::vector<DebugCube> DebugCube::cubes;

DebugCube::DebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
}

void DebugCube::draw(Shader &shader) {
	if (!DebugCube::initialized) {
		throw std::runtime_error("Debug cube wasn't initialized");
	}
	glBindVertexArray(DebugCube::VAO);
	glm::mat4 translationMat = glm::translate(glm::mat4(1), position);
	glm::mat4 rotationMat = glm::mat4(1);
	rotationMat = glm::rotate(rotationMat, glm::radians(rotation.x),
			glm::vec3(1, 0, 0));
	rotationMat = glm::rotate(rotationMat, glm::radians(rotation.y),
			glm::vec3(0, 1, 0));
	rotationMat = glm::rotate(rotationMat, glm::radians(rotation.z),
			glm::vec3(0, 0, 1));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);
	glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
	;
	shader.setMatrix4f("model", modelMat);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void DebugCube::init() {
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	DebugCube::VAO = VAO;
	DebugCube::VBO = VBO;
	DebugCube::EBO = EBO;

	float vertexCoords[24] = {
	// front
			-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
			// back
			-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0 };

	unsigned short indices[36] = { // front
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

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertexCoords,
	GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned short), indices,
	GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	DebugCube::initialized = true;
}
