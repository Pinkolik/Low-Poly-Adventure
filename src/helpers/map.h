#ifndef MAP_H
#define MAP_H
#include "camera.h"
#include "model.h"
#include "shader.h"
#include <nlohmann/json.hpp>
#include <vector>

struct ModelInfo {
  Model *model;
  std::vector<float> scale;
  std::vector<float> translate;
  std::vector<float> rotate;
};

class Map {
public:
  Map(const char *mapPath);
  void draw(Camera &camera, float aspect);

private:
  ModelInfo parseModelInfo(nlohmann::basic_json<> jModel);
  glm::mat4 getModelMat(ModelInfo &modelInfo);
  std::vector<ModelInfo> modelInfos;
  std::vector<ModelInfo> lightInfos;
  Shader *defaultShader;
  Shader *ligthShader;
};

#endif