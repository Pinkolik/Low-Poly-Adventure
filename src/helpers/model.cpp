#include "model.h"
#include "primitive.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

Model::Model(const char *path) { loadModel(path); }

void Model::draw(Shader &shader) {}

void Model::loadModel(const char *path) {
  tinygltf::TinyGLTF loader;
  tinygltf::Model gltfModel;
  string err;
  string warn;

  bool res = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, path);
  if (!warn.empty()) {
    cout << "WARN: " << warn << endl;
  }

  if (!err.empty()) {
    throw runtime_error("ERR: " + err);
  }

  if (!res) {
    throw runtime_error("Failed to load glTF: " + string(path));
  } else {
    cout << "Loaded glTF: " << path << endl;
  }

  tinygltf::Scene scene = gltfModel.scenes[gltfModel.defaultScene];
  for (size_t i = 0; i < scene.nodes.size(); i++) {
    tinygltf::Node gltfNode = gltfModel.nodes[scene.nodes[i]];
    Node node = processNode(gltfModel, gltfNode);
    nodes.push_back(node);
  }
}

Node Model::processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode) {
  Node node;
  vector<double> rotation = gltfNode.rotation;
  vector<double> scale = gltfNode.scale;
  vector<double> translation = gltfNode.translation;
  if (!rotation.empty()) {
    node.rotation =
        glm::vec4(rotation[0], rotation[1], rotation[2], rotation[3]);
  }
  if (!scale.empty()) {
    node.scale = glm::vec3(scale[0], scale[1], scale[2]);
  }
  if (!translation.empty()) {
    node.translation =
        glm::vec3(translation[0], translation[1], translation[2]);
  }

  tinygltf::Mesh gltfMesh = gltfModel.meshes[gltfNode.mesh];
  node.mesh = prcoessMesh(gltfModel, gltfMesh);

  for (size_t i = 0; i < gltfNode.children.size(); i++) {
    tinygltf::Node gltfChildNode = gltfModel.nodes[gltfNode.children[i]];
    Node childNode = processNode(gltfModel, gltfChildNode);
    node.children.push_back(childNode);
  }
  return node;
}

Mesh Model::prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh) {
  Mesh mesh;

  for (size_t i = 0; i < gltfMesh.primitives.size(); i++) {
    tinygltf::Primitive gltfPrimitive = gltfMesh.primitives[i];
    Primitive primitive;

    vector<vector<float>> normals;
    vector<vector<float>> positions;
    vector<vector<float>> texcoord;
    for (auto &attrib : gltfPrimitive.attributes) {
      std::cout << attrib.first << std::endl;
      unsigned int accessor = attrib.second;
      if (attrib.first.compare("NORMAL") == 0) {
        normals = getFloatArrayVector(gltfModel, accessor, 3);
      } else if (attrib.first.compare("POSITION") == 0) {
        positions = getFloatArrayVector(gltfModel, accessor, 3);
      } else if (attrib.first.compare("TEXCOORD_0") == 0) {
        texcoord = getFloatArrayVector(gltfModel, accessor, 2);
      }
    }
    assert(normals.size() == positions.size() &&
           positions.size() == texcoord.size());

    for (size_t j = 0; j < normals.size(); j++) {
      Vertex vertex;
      vertex.position =
          glm::vec3(positions[j][0], positions[j][1], positions[j][2]);
      vertex.normal = glm::vec3(normals[j][0], normals[j][1], normals[j][2]);
      vertex.texcoord = glm::vec2(texcoord[j][0], texcoord[j][1]);
      primitive.vertices.push_back(vertex);
    }
    primitive.indices =
        getUnsignedShortVector(gltfModel, gltfPrimitive.indices);
    mesh.primitives.push_back(primitive);
  }
  return mesh;
}

vector<vector<float>> Model::getFloatArrayVector(tinygltf::Model &gltfModel,
                                                 const unsigned int accessor,
                                                 size_t floatArrSize) {
  tinygltf::Accessor gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer buffer = gltfModel.buffers[bufferView.buffer];
  vector<vector<float>> res;
  for (size_t j = 0; j < gltfAccessor.count; j++) {
    unsigned char *c = buffer.data.data() + bufferView.byteOffset +
                       j * sizeof(float) * floatArrSize;

    float floatArray[floatArrSize];
    fillFloatArrayFromBytes(c, floatArray, floatArrSize);
    vector<float> floatVec;
    for (size_t k = 0; k < floatArrSize; k++) {
      floatVec.push_back(floatArray[k]);
    }
    res.push_back(floatVec);
  }
  return res;
}

void Model::fillFloatArrayFromBytes(const unsigned char *byteArray,
                                    float *floatArray, size_t floatArrSize) {
  for (size_t i = 0; i < floatArrSize; i++) {
    float f;
    mempcpy(&f, byteArray + i * sizeof(float), sizeof(float));
    floatArray[i] = f;
  }
}

vector<unsigned short>
Model::getUnsignedShortVector(tinygltf::Model &gltfModel,
                              const unsigned int accessor) {
  tinygltf::Accessor gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer buffer = gltfModel.buffers[bufferView.buffer];
  vector<unsigned short> res;
  for (size_t j = 0; j < gltfAccessor.count; j++) {
    unsigned char *c =
        buffer.data.data() + bufferView.byteOffset + j * sizeof(unsigned short);
    unsigned short s;
    memcpy(&s, c, sizeof(unsigned short));
    res.push_back(s);
  }
  return res;
}
