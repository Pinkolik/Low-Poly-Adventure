#ifndef MODEL_H
#define MODEL_H

#include "../../helpers/tiny_gltf.h"
#include "Mesh.h"
#include "Node.h"
#include "PositionStruct.h"
#include "Primitive.h"
#include "../../shader/Shader.h"
#include <vector>

class Model {
public:
    explicit Model(const char *path);

    void buffer();

    void draw(Shader &shader);

    void setTranslation(glm::vec3 translation);

    void setScale(glm::vec3 scale);

    void setRotation(glm::quat rotation);

    glm::vec3 getSpawnPos();

    std::vector<IntersectionResult *> getMinimumTranslationVec(Model &other);

private:
    std::vector<Node> nodes;

    PositionStruct position;

    void load(const char *path);

    Node processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode);

    Mesh processMesh(tinygltf::Model &gltfModel, tinygltf::Mesh &gltfMesh);

    static Texture processTexture(tinygltf::Model &gltfModel,
                                  unsigned int material);

    static std::vector<std::vector<float>>
    createFloatArrayVector(tinygltf::Model &gltfModel,
                           unsigned int accessor, size_t floatArrSize);

    std::vector<unsigned short>
    createUnsignedShortVector(tinygltf::Model &gltfModel,
                              unsigned int accessor);
};

#endif
