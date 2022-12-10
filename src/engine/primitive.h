#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include <vector>

using namespace std;

class Primitive {
public:
	Primitive(vector<Vertex> &vertices, vector<unsigned short> &indices,
			Texture &texture);
	void buffer();
	void draw(Shader &shader);
	Texture& getTexture();
	vector<Vertex>& getVertices();
	float findIntersectionCoefficient(glm::mat4 modelMat, glm::vec3 origin,
			glm::vec3 direction);

private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Texture texture;
};
#endif
