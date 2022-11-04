#include "map.h"
#include "camera.h"
#include "model.h"
#include "shader.h"
#include <fstream>
#include <glm/ext.hpp> // perspective, translate, rotate
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <map>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

Map::Map(const char *mapPath) {
  defaultShader = new Shader("./resources/shaders/vShader.glsl",
                             "./resources/shaders/fShader.glsl");
  ligthShader = new Shader("./resources/shaders/vShader.glsl",
                           "./resources/shaders/fShaderLight.glsl");
  std::ifstream jFile(mapPath);
  json jObj = json::parse(jFile);

  for (auto jModel : jObj["models"]) {
    ModelInfo info = parseModelInfo(jModel);
    modelInfos.push_back(info);
  }

  for (auto jModel : jObj["lights"]) {
    ModelInfo info = parseModelInfo(jModel);
    lightInfos.push_back(info);
  }
}

void Map::draw(Camera &camera, float aspect) {
  glm::mat4 projection =
      glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 100.0f);
  defaultShader->setMatrix4f("projection", projection);
  glm::mat4 view = camera.getViewMatrix();

  ligthShader->use();
  ligthShader->setMatrix4f("projection", projection);
  ligthShader->setMatrix4f("view", view);
  for (auto info : lightInfos) {
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = getModelMat(info);
    ligthShader->setMatrix4f("model", modelMat);
    info.model->draw(*ligthShader);
  }

  defaultShader->use();
  defaultShader->setInt("tex", 0);
  defaultShader->setMatrix4f("view", view);
  glm::vec3 lightPos =
      glm::vec3(lightInfos[0].translate[0], lightInfos[0].translate[1],
                lightInfos[0].translate[2]);
  defaultShader->setVec3f("lightPos", lightPos);
  defaultShader->setVec3f("viewPos", camera.getPosition());
  for (auto info : modelInfos) {
    glm::mat4 modelMat = getModelMat(info);
    defaultShader->setMatrix4f("model", modelMat);
    info.model->draw(*defaultShader);
  }
}

void Map::tick(float time) {
  lightInfos[0].translate[0] = 0.5 * cos(time);
  lightInfos[0].translate[1] = -0.5 + 0.2 * sin(time * 8);
  lightInfos[0].translate[2] = 0.5 * sin(time);
}

ModelInfo Map::parseModelInfo(nlohmann::basic_json<> jModel) {
  ModelInfo info;
  std::string modelName = std::string(jModel["model"]);
  std::string modelPath = "./resources/models/" + modelName + ".json";
  std::vector<float> scaleTex = jModel["scaleTex"];
  Model *model = new Model(modelPath.c_str(), scaleTex[0], scaleTex[1]);
  info.model = model;
  std::vector<float> scale = jModel["scale"];
  info.scale = scale;
  std::vector<float> translate = jModel["translate"];
  info.translate = translate;
  std::vector<float> rotate = jModel["rotate"];
  info.rotate = rotate;
  return info;
}

glm::mat4 Map::getModelMat(ModelInfo &info) {
  glm::mat4 modelMat = glm::mat4(1);
  modelMat =
      glm::translate(modelMat, glm::vec3(info.translate[0], info.translate[1],
                                         info.translate[2]));
  modelMat =
      glm::rotate(modelMat, glm::radians(info.rotate[0]), glm::vec3(1, 0, 0));
  modelMat =
      glm::rotate(modelMat, glm::radians(info.rotate[1]), glm::vec3(0, 1, 0));
  modelMat =
      glm::rotate(modelMat, glm::radians(info.rotate[2]), glm::vec3(0, 0, 1));
  modelMat = glm::scale(modelMat,
                        glm::vec3(info.scale[0], info.scale[1], info.scale[2]));
  return modelMat;
}