#ifndef MODEL_H
#define MODEL_H

#include "../../helpers/tiny_gltf.h"
#include "Mesh.h"
#include "Node.h"
#include "TransformationStruct.h"
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

    std::vector<glm::vec3 *> getMinimumTranslationVec(Model &other) const;

    const std::vector<Node> &getNodes() const;

private:
    std::vector<Node> nodes;
    std::vector<Mesh *> loadedMeshes;
    std::vector<Texture *> loadedTextures;

    TransformationStruct transform;

    void load(const char *path);

    Node processNode(tinygltf::Model &gltfModel, tinygltf::Node &gltfNode);

    Mesh *processMesh(tinygltf::Model &gltfModel, int mesh);

    Primitive processPrimitive(tinygltf::Model &gltfModel, tinygltf::Primitive &gltfPrimitive);

    Texture *processTexture(tinygltf::Model &gltfModel, unsigned int material);

};

#endif
