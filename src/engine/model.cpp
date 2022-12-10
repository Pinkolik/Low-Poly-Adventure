#include "model.h"

#include "vertex.h"
#include <iostream>
#include <stdexcept>
#include <string>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../helpers/tiny_gltf.h"

Model::Model(const char *path) { load(path); }

glm::vec3 Model::getSpawnPos() {
  for (Node &node : nodes) {
    if (!node.isSpawn()) {
      continue;
    }
    return node.getTranslation();
  }
  return glm::vec3(0);
}

void Model::buffer() {
  for (Node &node : nodes) {
    node.buffer();
  }
}

void Model::draw(Shader &shader) {
  for (Node &node : nodes) {
    // if (node.isSpawn()) {
    //   continue;
    // }
    node.draw(shader, rotation, scale, translation);
  }
}

void Model::load(const char *path) {
  tinygltf::TinyGLTF loader;
  tinygltf::Model gltfModel;
  std::string err;
  std::string warn;

  bool res = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path);
  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    throw std::runtime_error("ERR: " + err);
  }

  if (!res) {
    throw std::runtime_error("Failed to load glTF: " + std::string(path));
  } else {
    std::cout << "Loaded glTF: " << path << std::endl;
  }

  tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene];
  for (size_t i = 0; i < scene.nodes.size(); i++) {
    tinygltf::Node &gltfNode = gltfModel.nodes[scene.nodes[i]];
    Node node = processNode(gltfModel, gltfNode);
    nodes.push_back(node);
  }
}

Node Model::processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode) {
  std::vector<double> rotation = gltfNode.rotation;
  std::vector<double> scale = gltfNode.scale;
  std::vector<double> translation = gltfNode.translation;

  tinygltf::Mesh &gltfMesh = gltfModel.meshes[gltfNode.mesh];
  Mesh mesh = prcoessMesh(gltfModel, gltfMesh);

  Node node = Node(rotation, scale, translation, mesh);
  for (size_t i = 0; i < gltfNode.children.size(); i++) {
    tinygltf::Node &gltfChildNode = gltfModel.nodes[gltfNode.children[i]];
    Node childNode = processNode(gltfModel, gltfChildNode);
    node.addChild(childNode);
  }
  return node;
}

Mesh Model::prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh) {
  std::vector<Primitive> primitives;
  for (size_t i = 0; i < gltfMesh.primitives.size(); i++) {
    tinygltf::Primitive &gltfPrimitive = gltfMesh.primitives[i];

    std::vector<std::vector<float>> normals;
    std::vector<std::vector<float>> positions;
    std::vector<std::vector<float>> texcoord;
    for (auto &attrib : gltfPrimitive.attributes) {
      std::cout << attrib.first << std::endl;
      unsigned int accessor = attrib.second;
      if (attrib.first.compare("NORMAL") == 0) {
        normals = createFloatArrayVecctor(gltfModel, accessor, 3);
      } else if (attrib.first.compare("POSITION") == 0) {
        positions = createFloatArrayVecctor(gltfModel, accessor, 3);
      } else if (attrib.first.compare("TEXCOORD_0") == 0) {
        texcoord = createFloatArrayVecctor(gltfModel, accessor, 2);
      }
    }
    assert(normals.size() == positions.size() &&
           positions.size() == texcoord.size());

    std::vector<Vertex> vertices;
    for (size_t j = 0; j < normals.size(); j++) {
      Vertex vertex;
      vertex.position =
          glm::vec3(positions[j][0], positions[j][1], positions[j][2]);
      vertex.normal = glm::vec3(normals[j][0], normals[j][1], normals[j][2]);
      vertex.texcoord = glm::vec2(texcoord[j][0], texcoord[j][1]);
      vertices.push_back(vertex);
    }
    std::vector<unsigned short> indices =
        createUnsignedShortVector(gltfModel, gltfPrimitive.indices);
    Texture texture = processTexture(gltfModel, gltfPrimitive.material);
    Primitive primitive = Primitive(vertices, indices, texture);
    primitives.push_back(primitive);
  }
  Mesh mesh = Mesh(primitives);
  return mesh;
}

std::vector<std::vector<float>>
Model::createFloatArrayVecctor(tinygltf::Model &gltfModel,
                               const unsigned int accessor,
                               size_t floatArrSize) {
  tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView &bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
  std::vector<std::vector<float>> res;
  for (size_t j = 0; j < gltfAccessor.count; j++) {
    unsigned char *c = buffer.data.data() + bufferView.byteOffset +
                       j * sizeof(float) * floatArrSize;

    float floatArray[floatArrSize];
    mempcpy(floatArray, c, sizeof(float) * floatArrSize);
    std::vector<float> floatVec;
    for (size_t k = 0; k < floatArrSize; k++) {
      floatVec.push_back(floatArray[k]);
    }
    res.push_back(floatVec);
  }
  return res;
}

std::vector<unsigned short>
Model::createUnsignedShortVector(tinygltf::Model &gltfModel,
                                 const unsigned int accessor) {
  tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
  tinygltf::BufferView &bufferView =
      gltfModel.bufferViews[gltfAccessor.bufferView];
  tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
  std::vector<unsigned short> res;
  for (size_t j = 0; j < gltfAccessor.count; j++) {
    unsigned char *c =
        buffer.data.data() + bufferView.byteOffset + j * sizeof(unsigned short);
    unsigned short s;
    memcpy(&s, c, sizeof(unsigned short));
    res.push_back(s);
  }
  return res;
}

Texture Model::processTexture(tinygltf::Model &gltfModel,
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

  Texture tex{texId, gltfImage.name};
  return tex;
}

void Model::setTranslation(glm::vec3 translation) {
  this->translation = translation;
}

void Model::setScale(glm::vec3 scale) { this->scale = scale; }

void Model::setRotation(glm::quat rotation) { this->rotation = rotation; }