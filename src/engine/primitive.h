#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "glm/fwd.hpp"
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include <vector>

class Primitive {
public:
  Primitive(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices,
            Texture &texture);
  void buffer();
  void draw(Shader &shader);

  glm::vec3 *getMinimumTranslationVec(glm::mat4 modelMat, Primitive other,
                                      glm::mat4 otherModelMat);

  Texture &getTexture();
  std::vector<Vertex> &getVertices();

private:
  unsigned int VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<unsigned short> indices;
  Texture texture;

  std::vector<glm::vec3> getTriangleVertices(int idx, glm::mat4 modelMat);
  std::vector<glm::vec3> getTriangleNormals(int idx, glm::mat4 modelMat);
};
#endif
