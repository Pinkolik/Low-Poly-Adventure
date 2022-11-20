#ifndef MAP_H
#define MAP_H
#include "../helpers/tiny_gltf.h"
#include "mesh.h"
#include "node.h"
#include "primitive.h"
#include "shader.h"
#include <vector>

class Map {
public:
  Map(const char *path);
  void bufferMap();
  void draw(Shader &shader);
  glm::vec3 getSpawnPos();
  float findIntersectionCoefficient(glm::vec3 origin, glm::vec3 direction);
  float findIntersectionCoefficient(Node &anotherNode, glm::vec3 direction);

private:
  vector<Node> nodes;
  void loadModel(const char *path);
  Node processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode);
  Mesh prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh);
  vector<vector<float>> getFloatArrayVector(tinygltf::Model &gltfModel,
                                            const unsigned int accessor,
                                            size_t floatArrSize);
  vector<unsigned short> getUnsignedShortVector(tinygltf::Model &gltfModel,
                                                const unsigned int accessor);
  Texture getTexture(tinygltf::Model &gltfModel, const unsigned int material);
  void logVector(const char *prefix, glm::vec3 vec);
};

#endif