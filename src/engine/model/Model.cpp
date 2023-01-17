#include "Model.h"

#include "Vertex.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>
#include "../../helpers/tiny_gltf.h"
#include "ModelUtil.h"

Model::Model(const char *path) { load(path); }

std::vector<glm::vec3 *> Model::getMinimumTranslationVec(Model &other) const {
    std::vector<glm::vec3 *> res;
    glm::mat4 transMat = ModelUtil::getTransMat(transform);
    glm::mat4 otherTransMat = ModelUtil::getTransMat(other.transform);
    for (const auto &node: nodes) {
        for (const auto &otherNode: other.nodes) {
            bool isAabbIntersecting = node.isAABBIntersecting(transMat, otherNode, otherTransMat);
            if (!isAabbIntersecting) {
                continue;
            }
            std::vector<glm::vec3 *> mtvs = node.getMinimumTranslationVec(transMat, otherNode, otherTransMat);
            if (!mtvs.empty()) {
                res.insert(res.end(), mtvs.begin(), mtvs.end());
            }
        }
    }
    return res;
}

void Model::buffer() {
    for (Node &node: nodes) {
        node.buffer();
    }
}

void Model::draw(Shader &shader) {
    glm::mat4 transMat = ModelUtil::getTransMat(transform);
    for (Node &node: nodes) {
        node.draw(shader, transMat);
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
    for (int i: scene.nodes) {
        tinygltf::Node &gltfNode = gltfModel.nodes[i];
        Node node = processNode(gltfModel, gltfNode);
        nodes.push_back(node);
    }

    glm::mat4 transMat = glm::mat4(1);
    for (auto &node: nodes) {
        node.calculateAABBs(transMat);
    }
}

Node Model::processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode) {
    std::vector<double> rotation = gltfNode.rotation;
    std::vector<double> scale = gltfNode.scale;
    std::vector<double> translation = gltfNode.translation;

    Mesh *mesh = nullptr;
    if (gltfNode.mesh != -1) {
        mesh = processMesh(gltfModel, gltfNode.mesh);
    }

    Node node = Node(rotation, scale, translation, mesh);
    for (int i: gltfNode.children) {
        tinygltf::Node &gltfChildNode = gltfModel.nodes[i];
        Node childNode = processNode(gltfModel, gltfChildNode);
        node.addChild(childNode);
    }
    return node;
}

Mesh *Model::processMesh(tinygltf::Model &gltfModel, int meshId) {
    for (auto &loadedMesh: loadedMeshes) {
        if (loadedMesh->getId() == meshId) {
            return loadedMesh;
        }
    }
    tinygltf::Mesh &gltfMesh = gltfModel.meshes[meshId];
    std::vector<Primitive> primitives;
    for (auto &gltfPrimitive: gltfMesh.primitives) {
        Primitive primitive = processPrimitive(gltfModel, gltfPrimitive);
        primitives.push_back(primitive);
    }
    Mesh *pMesh = new Mesh(meshId, primitives);
    loadedMeshes.push_back(pMesh);
    return pMesh;
}

Primitive Model::processPrimitive(tinygltf::Model &gltfModel, tinygltf::Primitive &gltfPrimitive) {
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<float>> positions;
    std::vector<std::vector<float>> texcoord;
    for (auto &attrib: gltfPrimitive.attributes) {
        unsigned int accessor = attrib.second;
        if (attrib.first == "NORMAL") {
            normals = ModelUtil::createFloatArrayVector(gltfModel, accessor, 3);
        } else if (attrib.first == "POSITION") {
            positions = ModelUtil::createFloatArrayVector(gltfModel, accessor, 3);
        } else if (attrib.first == "TEXCOORD_0") {
            texcoord = ModelUtil::createFloatArrayVector(gltfModel, accessor, 2);
        }
    }
    assert(normals.size() == positions.size() && positions.size() == texcoord.size());

    std::vector<Vertex> vertices;
    for (size_t j = 0; j < normals.size(); j++) {
        Vertex vertex{};
        vertex.position = glm::vec3(positions[j][0], positions[j][1], positions[j][2]);
        vertex.normal = glm::vec3(normals[j][0], normals[j][1], normals[j][2]);
        vertex.texCoord = glm::vec2(texcoord[j][0], texcoord[j][1]);
        vertices.push_back(vertex);
    }
    std::vector<unsigned short> indices = ModelUtil::createUnsignedShortVector(gltfModel, gltfPrimitive.indices);
    Texture *texture = processTexture(gltfModel, gltfPrimitive.material);
    Primitive primitive = Primitive(vertices, indices, texture);
    return primitive;
}

Texture *Model::processTexture(tinygltf::Model &gltfModel,
                               const unsigned int material) {
    tinygltf::Material &gltfMaterial = gltfModel.materials[material];
    int textureIdx = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;

    tinygltf::Texture &gltfTexture = gltfModel.textures[textureIdx];
    for (auto &texture: loadedTextures) {
        if (texture->source == gltfTexture.source) {
            return texture;
        }
    }
    tinygltf::Image &gltfImage = gltfModel.images[gltfTexture.source];

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
    } else {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, gltfImage.image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto *tex = new Texture{gltfTexture.source, texId, gltfImage.name};
    loadedTextures.push_back(tex);
    return tex;
}

void Model::setTranslation(glm::vec3 translation) {
    transform.translation = translation;
}

void Model::setScale(glm::vec3 scale) { transform.scale = scale; }

const std::vector<Node> &Model::getNodes() const {
    return nodes;
}
