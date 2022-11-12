#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "node.h"
#include "primitive.h"
#include "shader.h"
#include "tiny_gltf.h"
#include <vector>

class Model {
public:
  Model(const char *path);
  void bufferModel();
  void draw(Shader &shader);

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
  void bufferNode(Node &node);
  void bufferPrimitive(Primitive &primitive);
  void drawNode(Shader &shader, Node &node);
  void drawPrimitive(Shader &shader, Primitive &primitive);
};

#endif