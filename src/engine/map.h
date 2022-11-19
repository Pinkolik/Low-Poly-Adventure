#ifndef MAP_H
#define MAP_H
#include "../helpers/tiny_gltf.h"
#include "mesh.h"
#include "node.h"
#include "primitive.h"
#include "shader.h"
#include <vector>

struct DebugCube {
  unsigned int VAO, VBO, EBO;
};

class Map {
public:
  Map(const char *path);
  void bufferMap();
  void draw(Shader &shader);
  glm::vec3 getSpawnPos();
  glm::vec3 *findIntersection(glm::vec3 origin, glm::vec3 direction);

private:
  vector<glm::vec3> debugCubePoses;
  DebugCube debugCube;
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
  void bufferNode(Node &node);
  void bufferPrimitive(Primitive &primitive);
  void drawNode(Shader &shader, Node &node);
  void drawPrimitive(Shader &shader, Primitive &primitive);
  glm::vec3 *findIntersection(Node &node, glm::vec3 origin,
                              glm::vec3 direction);
  glm::vec3 *findIntersection(Primitive &primitive, glm::mat4 modelMat,
                              glm::vec3 origin, glm::vec3 direction);
  glm::mat4 getModelMatForNode(Node &node);
  bool isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                             glm::vec3 normal, glm::vec3 point);
  void logVector(const char *prefix, glm::vec3 vec);
  void bufferDebugCube();
  void drawDebugCubes(Shader &shader);
  glm::vec3 *getMinDistanceToOriginVector(vector<glm::vec3 *> &points,
                                          glm::vec3 origin);
};

#endif