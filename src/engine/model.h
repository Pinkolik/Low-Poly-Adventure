#ifndef MODEL_H
#define MODEL_H
#include "../helpers/tiny_gltf.h"
#include "mesh.h"
#include "node.h"
#include "primitive.h"
#include "shader.h"
#include <vector>

class Model {
public:
  Model(const char *path);

  void buffer();
  void draw(Shader &shader);

  void setTranslation(glm::vec3 translation);
  void setScale(glm::vec3 scale);
  void setRotation(glm::quat rotation);

  glm::vec3 getSpawnPos();

private:
  std::vector<Node> nodes;

  glm::quat rotation = glm::quat(1, 0, 0, 0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 translation = glm::vec3(0);

  void load(const char *path);

  Node processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode);
  Mesh prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh);
  Texture processTexture(tinygltf::Model &gltfModel,
                         const unsigned int material);

  std::vector<std::vector<float>>
  createFloatArrayVecctor(tinygltf::Model &gltfModel,
                          const unsigned int accessor, size_t floatArrSize);
  std::vector<unsigned short>
  createUnsignedShortVector(tinygltf::Model &gltfModel,
                            const unsigned int accessor);
};

#endif
