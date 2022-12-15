#ifndef MODEL_H
#define MODEL_H

#include "../helpers/tiny_gltf.h"
#include "mesh.h"
#include "node.h"
#include "position_struct.h"
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

    glm::vec3 *getMinimumTranslationVec(Model &other);

private:
    std::vector<Node> nodes;

    PositionStruct position;

    void load(const char *path);

    Node processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode);

    Mesh processMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh);

    Texture processTexture(tinygltf::Model &gltfModel,
                           const unsigned int material);

    std::vector<std::vector<float>>
    createFloatArrayVector(tinygltf::Model &gltfModel,
                           const unsigned int accessor, size_t floatArrSize);

    std::vector<unsigned short>
    createUnsignedShortVector(tinygltf::Model &gltfModel,
                              const unsigned int accessor);
};

#endif
