#include "model.h"
#include "primitive.h"
#include <cstddef>
#include <exception>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

Model::Model(const char *path) { loadModel(path); }

void Model::bufferModel() {
  for (Node &node : nodes) {
    bufferNode(node);
  }
}

void Model::draw(Shader &shader) {
  for (Node &node : nodes) {
    drawNode(shader, node);
  }
}

void Model::drawNode(Shader &shader, Node &node) {
  glm::mat4 modelMat = glm::mat4(1);
  modelMat = glm::translate(modelMat, node.translation);
  modelMat = glm::scale(modelMat, node.scale);
  shader.setMatrix4f("model", modelMat);
  for (Primitive &primitive : node.mesh.primitives) {
    drawPrimitive(shader, primitive);
  }

  for (Node &childNode : node.children) {
    drawNode(shader, childNode);
  }
}

void Model::drawPrimitive(Shader &shader, Primitive &primitive) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, primitive.texture.id);
  glBindVertexArray(primitive.VAO);
  glDrawElements(GL_TRIANGLES, primitive.indices.size(), GL_UNSIGNED_SHORT, 0);
}

void Model::bufferNode(Node &node) {
  for (Primitive &primitive : node.mesh.primitives) {
    bufferPrimitive(primitive);
  }

  for (Node &childNode : node.children) {
    bufferNode(childNode);
  }
}

void Model::bufferPrimitive(Primitive &primitive) {
  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  primitive.VAO = VAO;
  primitive.VBO = VBO;
  primitive.EBO = EBO;

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, primitive.vertices.size() * sizeof(Vertex),
               primitive.vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               primitive.indices.size() * sizeof(unsigned short),
               primitive.indices.data(), GL_STATIC_DRAW);

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

void Model::loadModel(const char *path) {
  tinygltf::TinyGLTF loader;
  tinygltf::Model gltfModel;
  string err;
  string warn;

  bool res = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path);
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

  tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene];
  for (size_t i = 0; i < scene.nodes.size(); i++) {
    tinygltf::Node &gltfNode = gltfModel.nodes[scene.nodes[i]];
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

  tinygltf::Mesh &gltfMesh = gltfModel.meshes[gltfNode.mesh];
  node.mesh = prcoessMesh(gltfModel, gltfMesh);

  for (size_t i = 0; i < gltfNode.children.size(); i++) {
    tinygltf::Node &gltfChildNode = gltfModel.nodes[gltfNode.children[i]];
    Node childNode = processNode(gltfModel, gltfChildNode);
    node.children.push_back(childNode);
  }
  return node;
}

Mesh Model::prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh) {
  Mesh mesh;

  for (size_t i = 0; i < gltfMesh.primitives.size(); i++) {
    tinygltf::Primitive &gltfPrimitive = gltfMesh.primitives[i];
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
    primitive.texture = getTexture(gltfModel, gltfPrimitive.material);
    mesh.primitives.push_back(primitive);
  }
  return mesh;
}

vector<vector<float>> Model::getFloatArrayVector(tinygltf::Model &gltfModel,
                                                 const unsigned int accessor,
                                                 size_t floatArrSize) {
  tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView &bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
  vector<vector<float>> res;
  for (size_t j = 0; j < gltfAccessor.count; j++) {
    unsigned char *c = buffer.data.data() + bufferView.byteOffset +
                       j * sizeof(float) * floatArrSize;

    float floatArray[floatArrSize];
    mempcpy(floatArray, c, sizeof(float) * floatArrSize);
    vector<float> floatVec;
    for (size_t k = 0; k < floatArrSize; k++) {
      floatVec.push_back(floatArray[k]);
    }
    res.push_back(floatVec);
  }
  return res;
}

vector<unsigned short>
Model::getUnsignedShortVector(tinygltf::Model &gltfModel,
                              const unsigned int accessor) {
  tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView &bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
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

Texture Model::getTexture(tinygltf::Model &gltfModel,
                          const unsigned int material) {
  tinygltf::Material &gltfMaterial = gltfModel.materials[material];
  int textureIdx = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
  tinygltf::Texture &gltfTexture = gltfModel.textures[textureIdx];
  tinygltf::Image &gltfImage = gltfModel.images[gltfTexture.source];
  size_t size = gltfImage.image.size();
  std::cout << "Image " << gltfImage.name << " size " << size << std::endl;

  unsigned int texId;

  glGenTextures(1, &texId);

  int width, height, nrComponents;

  nrComponents = gltfImage.component;
  width = gltfImage.width;
  height = gltfImage.height;
  GLenum format;
  if (nrComponents == 1) {
    format = GL_RED;
  } else if (nrComponents == 3) {
    format = GL_RGB;
  } else if (nrComponents == 4) {
    format = GL_RGBA;
  }

  glBindTexture(GL_TEXTURE_2D, texId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
               GL_UNSIGNED_BYTE, gltfImage.image.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Texture tex{texId};
  return tex;
}
