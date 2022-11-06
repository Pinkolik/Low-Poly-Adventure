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

struct LightInfo {
  Model *model;

  glm::vec3 position;

  float constant;
  float linear;
  float quadratic;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

class Map {
public:
  Map(const char *mapPath);
  void draw(Camera &camera, float aspect);
  void tick(float time);

private:
  ModelInfo parseModelInfo(nlohmann::basic_json<> jModel);
  LightInfo parseLightInfo(nlohmann::basic_json<> jLight);
  glm::mat4 getModelMat(ModelInfo &modelInfo);
  std::vector<ModelInfo> modelInfos;
  std::vector<LightInfo> lightInfos;
  Shader *defaultShader;
  Shader *ligthShader;
};

#endif