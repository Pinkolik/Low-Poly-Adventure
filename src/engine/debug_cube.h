#ifndef DEBUG_CUBE_H
#define DEBUG_CUBE_H
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>

class DebugCube {
public:
	DebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation =
			glm::vec3(0));
	void draw(Shader &shader);
	static void init();

	static std::vector<DebugCube> cubes;

private:
	static bool initialized;
	static unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

#endif
