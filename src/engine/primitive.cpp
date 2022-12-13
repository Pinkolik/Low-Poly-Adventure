#include "primitive.h"
#include "intersection/intersection_util.h"
#include "texture.h"
#include <glm/vec3.hpp>

Primitive::Primitive(std::vector<Vertex> &vertices,
                     std::vector<unsigned short> &indices, Texture &texture)
    : vertices(vertices), indices(indices), texture(texture) {}

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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texcoord));

  glBindVertexArray(0);
}

void Primitive::draw(Shader &shader) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}

Texture &Primitive::getTexture() { return texture; }

std::vector<Vertex> &Primitive::getVertices() { return vertices; }

bool Primitive::isIntersecting(glm::mat4 modelMat, Primitive other,
                               glm::mat4 otherModelMat) {
  for (int i = 0; i < indices.size(); i += 2) {
    std::vector<glm::vec3> firstTriangle;
    firstTriangle.push_back(modelMat *
                            glm::vec4(vertices[indices[i]].position, 1.0f));
    firstTriangle.push_back(modelMat *
                            glm::vec4(vertices[indices[i + 1]].position, 1.0f));
    firstTriangle.push_back(modelMat *
                            glm::vec4(vertices[indices[i + 2]].position, 1.0f));
    for (int j = 0; j < other.indices.size(); j += 2) {
      std::vector<glm::vec3> secondTriangle;
      secondTriangle.push_back(
          otherModelMat *
          glm::vec4(other.vertices[other.indices[j]].position, 1.0f));
      secondTriangle.push_back(
          otherModelMat *
          glm::vec4(other.vertices[other.indices[j + 1]].position, 1.0f));
      secondTriangle.push_back(
          otherModelMat *
          glm::vec4(other.vertices[other.indices[j + 2]].position, 1.0f));
      if (IntersectionUtil::isIntersecting(firstTriangle, secondTriangle)) {
        return true;
      }
    }
  }
  return false;
}