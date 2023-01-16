//
// Created by pinkolik on 1/8/23.
//

#include "ModelUtil.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../../helpers/tiny_gltf.h"

std::vector<std::vector<float>> ModelUtil::createFloatArrayVector(tinygltf::Model &gltfModel,
                                                                  const unsigned int accessor,
                                                                  size_t floatArrSize) {
    tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
    tinygltf::BufferView &bufferView = gltfModel.bufferViews[gltfAccessor.bufferView];
    tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
    std::vector<std::vector<float>> res;
    for (size_t j = 0; j < gltfAccessor.count; j++) {
        unsigned char *c = buffer.data.data() + bufferView.byteOffset + j * sizeof(float) * floatArrSize;

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

std::vector<unsigned short> ModelUtil::createUnsignedShortVector(tinygltf::Model &gltfModel,
                                                                 const unsigned int accessor) {
    tinygltf::Accessor &gltfAccessor = gltfModel.accessors[accessor];
    tinygltf::BufferView &bufferView = gltfModel.bufferViews[gltfAccessor.bufferView];
    tinygltf::Buffer &buffer = gltfModel.buffers[bufferView.buffer];
    std::vector<unsigned short> res;
    for (size_t j = 0; j < gltfAccessor.count; j++) {
        unsigned char *c = buffer.data.data() + bufferView.byteOffset + j * sizeof(unsigned short);
        unsigned short s;
        memcpy(&s, c, sizeof(unsigned short));
        res.push_back(s);
    }
    return res;
}

glm::mat4 ModelUtil::getTransMat(TransformationStruct transform) {
    glm::mat4 translationMat = glm::translate(glm::mat4(1), transform.translation);
    glm::mat4 rotationMat = glm::toMat4(transform.rotation);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), transform.scale);
    glm::mat4 modelMat = translationMat * rotationMat * scaleMat;
    return modelMat;
}

glm::mat4 ModelUtil::getTransMatWithoutRotation(TransformationStruct transform) {
    glm::mat4 translationMat = glm::translate(glm::mat4(1), transform.translation);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), transform.scale);
    glm::mat4 modelMat = translationMat * scaleMat;
    return modelMat;
}
