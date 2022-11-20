#include "map.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "primitive.h"
#include <cmath>
#include <cstddef>
#include <exception>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../helpers/tiny_gltf.h"

Map::Map(const char *path) { loadModel(path); }

glm::vec3 Map::getSpawnPos() {
  for (Node &node : nodes) {
    if (!node.isSpawn) {
      continue;
    }
    return node.translation;
  }
  return glm::vec3(0);
}

void Map::bufferMap() {
  for (Node &node : nodes) {
    bufferNode(node);
  }
}

void Map::draw(Shader &shader) {
  for (Node &node : nodes) {
    if (node.isSpawn) {
      continue;
    }
    drawNode(shader, node);
  }
}

glm::vec3 *Map::findIntersection(glm::vec3 origin, glm::vec3 direction) {
  vector<glm::vec3 *> intersections;
  for (Node &node : nodes) {
    glm::vec3 *intersection = findIntersection(node, origin, direction);
    if (intersection == NULL) {
      continue;
    }
    intersections.push_back(intersection);
  }
  glm::vec3 *result = getMinDistanceToOriginVector(intersections, origin);
  return result;
}

glm::vec3 *Map::findIntersection(Node &node, glm::vec3 origin,
                                 glm::vec3 direction) {
  vector<glm::vec3 *> intersections;
  glm::mat4 modelMat = getModelMatForNode(node);
  for (Primitive &primitive : node.mesh.primitives) {
    glm::vec3 *intersection =
        findIntersection(primitive, modelMat, origin, direction);
    if (intersection == NULL) {
      continue;
    }
    intersections.push_back(intersection);
  }

  for (Node &childNode : node.children) {
    glm::vec3 *intersection = findIntersection(childNode, origin, direction);
    if (intersection == NULL) {
      continue;
    }
    intersections.push_back(intersection);
  }

  return getMinDistanceToOriginVector(intersections, origin);
}

glm::vec3 *Map::findIntersection(Primitive &primitive, glm::mat4 modelMat,
                                 glm::vec3 origin, glm::vec3 direction) {
  vector<glm::vec3 *> intersections;
  glm::mat3 normalMat = glm::inverseTranspose(modelMat);
  for (int i = 0; i < primitive.indices.size(); i += 3) {
    glm::vec3 normal =
        normalMat * primitive.vertices[primitive.indices[i]].normal;
    glm::vec3 a =
        modelMat *
        glm::vec4(primitive.vertices[primitive.indices[i]].position, 1);
    glm::vec3 b =
        modelMat *
        glm::vec4(primitive.vertices[primitive.indices[i + 1]].position, 1);
    glm::vec3 c =
        modelMat *
        glm::vec4(primitive.vertices[primitive.indices[i + 2]].position, 1);

    float surfaceDistanceFromOrigin = -glm::dot(normal, a);
    float dotNormalDirection = glm::dot(normal, direction);
    if (dotNormalDirection == 0.f) {
      continue; // vector is parallel to surface
    }
    float vectorCoefficient =
        -(glm::dot(normal, origin) + surfaceDistanceFromOrigin) /
        dotNormalDirection;
    if (vectorCoefficient < 0.f) {
      continue; // vector is behind surface
    }
    glm::vec3 *intersection =
        new glm::vec3(origin + vectorCoefficient * direction);

    if (!isPointInsideTriangle(a, b, c, normal, *intersection)) {
      continue;
    }
    intersections.push_back(intersection);
  }
  return getMinDistanceToOriginVector(intersections, origin);
}

glm::vec3 *Map::getMinDistanceToOriginVector(vector<glm::vec3 *> &points,
                                             glm::vec3 origin) {
  glm::vec3 *result = NULL;
  if (points.empty()) {
    return NULL;
  }
  float minDistance = MAXFLOAT;
  for (auto point : points) {
    float distance = glm::distance(*point, origin);
    if (distance < minDistance) {
      minDistance = distance;
      delete result;
      result = point;
    }
  }
  return result;
}

bool Map::isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                                glm::vec3 normal, glm::vec3 point) {
  glm::vec3 edge0 = b - a;
  glm::vec3 edge1 = c - b;
  glm::vec3 edge2 = a - c;
  glm::vec3 c0 = point - a;
  glm::vec3 c1 = point - b;
  glm::vec3 c2 = point - c;

  return glm::dot(normal, glm::cross(edge0, c0)) > 0 &&
         glm::dot(normal, glm::cross(edge1, c1)) > 0 &&
         glm::dot(normal, glm::cross(edge2, c2)) > 0;
}

void Map::logVector(const char *prefix, glm::vec3 vec) {
  std::cout << prefix << " " << vec.x << "," << vec.y << "," << vec.z
            << std::endl;
}

void Map::drawNode(Shader &shader, Node &node) {
  glm::mat4 modelMat = getModelMatForNode(node);
  shader.setMatrix4f("model", modelMat);
  for (Primitive &primitive : node.mesh.primitives) {
    drawPrimitive(shader, primitive);
  }

  for (Node &childNode : node.children) {
    drawNode(shader, childNode);
  }
}

glm::mat4 Map::getModelMatForNode(Node &node) {
  glm::mat4 translation = glm::translate(glm::mat4(1), node.translation);
  glm::mat4 rotation = glm::toMat4(node.rotation);
  glm::mat4 scale = glm::scale(glm::mat4(1), node.scale);
  glm::mat4 modelMat = translation * rotation * scale;
  return modelMat;
}

void Map::drawPrimitive(Shader &shader, Primitive &primitive) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, primitive.texture.id);
  glBindVertexArray(primitive.VAO);
  glDrawElements(GL_TRIANGLES, primitive.indices.size(), GL_UNSIGNED_SHORT, 0);
}

void Map::bufferNode(Node &node) {
  for (Primitive &primitive : node.mesh.primitives) {
    bufferPrimitive(primitive);
  }

  for (Node &childNode : node.children) {
    bufferNode(childNode);
  }
}

void Map::bufferPrimitive(Primitive &primitive) {
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

void Map::loadModel(const char *path) {
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

Node Map::processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode) {
  Node node;
  vector<double> rotation = gltfNode.rotation;
  vector<double> scale = gltfNode.scale;
  vector<double> translation = gltfNode.translation;
  if (!rotation.empty()) {
    node.rotation =
        glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
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
  for (auto &primitive : node.mesh.primitives) {
    if (primitive.texture.name.compare("spawn") == 0) {
      node.isSpawn = true;
    }
  }

  for (size_t i = 0; i < gltfNode.children.size(); i++) {
    tinygltf::Node &gltfChildNode = gltfModel.nodes[gltfNode.children[i]];
    Node childNode = processNode(gltfModel, gltfChildNode);
    node.children.push_back(childNode);
  }
  return node;
}

Mesh Map::prcoessMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh) {
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

vector<vector<float>> Map::getFloatArrayVector(tinygltf::Model &gltfModel,
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
Map::getUnsignedShortVector(tinygltf::Model &gltfModel,
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

Texture Map::getTexture(tinygltf::Model &gltfModel,
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