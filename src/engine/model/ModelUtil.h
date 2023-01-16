//
// Created by pinkolik on 1/8/23.
//

#ifndef GAME_MODELUTIL_H
#define GAME_MODELUTIL_H

#include <vector>
#include "../../helpers/tiny_gltf.h"
#include "TransformationStruct.h"

class ModelUtil {
public:
    static std::vector<std::vector<float>>
    createFloatArrayVector(tinygltf::Model &gltfModel, unsigned int accessor, size_t floatArrSize);

    static std::vector<unsigned short> createUnsignedShortVector(tinygltf::Model &gltfModel, unsigned int accessor);

    static glm::mat4 getTransMat(TransformationStruct transform);

    static glm::mat4 getTransMatWithoutRotation(TransformationStruct transform);
};


#endif //GAME_MODELUTIL_H
