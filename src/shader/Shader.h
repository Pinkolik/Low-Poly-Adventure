#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <string>

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMatrix4f(const std::string &name, const glm::mat4 &value) const;

    void setVec3f(const std::string &name, const glm::vec3 &value) const;

private:
    unsigned int ID;

    static unsigned int compileShader(GLenum type, std::string &shaderSourceStr);

    static unsigned int linkProgram(unsigned int vertexShader,
                             unsigned int fragmentShader);
};

#endif
