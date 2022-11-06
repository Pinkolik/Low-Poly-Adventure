#ifndef MESH_H
#define MESH_H
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include <vector>

class Mesh {
public:
  // mesh data
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void draw(Shader &shader);

private:
  //  render data
  unsigned int VAO, VBO, EBO;

  void setupMesh();
};
#endif