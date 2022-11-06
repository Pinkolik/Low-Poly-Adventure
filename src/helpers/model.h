#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

class Model {
public:
  Model(const char *path);
  void draw(Shader &shader);

private:
  // model data
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> loadedTextures; 

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
  unsigned int loadTextureFromFile(const char *path);
};

#endif