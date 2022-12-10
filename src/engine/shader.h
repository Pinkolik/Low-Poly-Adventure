#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
	Shader(const char *vertexPath, const char *fragmentPath);

	void use();
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setMatrix4f(const std::string &name, const glm::mat4 &value);
	void setVec3f(const std::string &name, const glm::vec3 &value);

private:
	unsigned int ID;

	unsigned int compileShader(GLenum type, std::string &shaderSourceStr);
	unsigned int linkProgram(unsigned int vertexShader,
			unsigned int fragmentShader);
};

#endif
