#include "model.h"
#include "assimp/material.h"
#include "assimp/metadata.h"
#include "assimp/types.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>

Model::Model(const char *path) { loadModel(path); }

void Model::draw(Shader &shader) {
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader);
  }
}

void Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_TransformUVCoords);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error(importer.GetErrorString());
  }
  unsigned int len = scene->mMetaData->mNumProperties;
  for (unsigned int i = 0; i < len; i++) {
    aiMetadataEntry entry = scene->mMetaData->mValues[i];
    aiMetadataType type = entry.mType;
    aiString key = scene->mMetaData->mKeys[i];
    std::cout << "Key: " << key.C_Str() << " Type: " << type << std::endl;
  }
  float factor;
  scene->mMetaData->Get("UnitScaleFactor", factor);
  std::cout << "Scale factor: " << factor << std::endl;
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    // process vertex positions, normals and texture coordinates
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vec;
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }
  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
  // process material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    for (unsigned int j = 0; j < material->mNumProperties; j++) {
      aiMaterialProperty *property = material->mProperties[j];
      unsigned int length = property->mDataLength;
      std::stringstream dataStream;
      for (unsigned int k = 0; k < length; k++) {
        dataStream << (0xff & (unsigned int)property->mData[k]) << " ";
      }
      std::cout << "Key: " << property->mKey.data
                << " Type: " << property->mType
                << " Index: " << property->mIndex
                << " Semantic: " << property->mSemantic
                << " Data Length: " << length << " Data: " << dataStream.str()
                << std::endl;
    }
    aiUVTransform transform;
    material->Get(AI_MATKEY_UVTRANSFORM_DIFFUSE(0), transform);
    std::vector<Texture> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    mat->Get(AI_MATKEY_NAME, str);
    bool skip = false;
    for (unsigned int j = 0; j < loadedTextures.size(); j++) {
      if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(loadedTextures[j]);
        skip = true;
        break;
      }
    }
    if (!skip) { // if texture hasn't been loaded already, load it
      Texture texture;
      texture.id = loadTextureFromFile(str.C_Str());
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      loadedTextures.push_back(texture); // add to loaded textures
    }
  }
  return textures;
}

unsigned int Model::loadTextureFromFile(const char *path) {
  // stbi_set_flip_vertically_on_load(true);
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1) {
      format = GL_RED;
    } else if (nrComponents == 3) {
      format = GL_RGB;
    } else if (nrComponents == 4) {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    stbi_image_free(data);
    throw std::runtime_error("Failed to load texture");
  }

  return textureID;
}