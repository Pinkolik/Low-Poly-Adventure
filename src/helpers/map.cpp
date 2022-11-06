#include "map.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
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

  for (auto jLight : jObj["lights"]) {
    LightInfo info = parseLightInfo(jLight);
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
    modelMat = glm::translate(modelMat, info.position);
    modelMat = glm::scale(modelMat, glm::vec3(0.1, 0.1, 0.1));
    ligthShader->setMatrix4f("model", modelMat);
    ligthShader->setVec3f("aColor", info.specular);
    info.model->draw(*ligthShader);
  }

  // glm::vec3 lightPos =
  //     glm::vec3(lightInfos[0].translate[0], lightInfos[0].translate[1],
  //               lightInfos[0].translate[2]);

  defaultShader->use();
  defaultShader->setInt("material.diffuse", 0);
  defaultShader->setInt("material.specular", 1);
  defaultShader->setFloat("material.shininess", 32.0f);

  defaultShader->setVec3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
  defaultShader->setVec3f("dirLight.ambient", glm::vec3(0.1, 0.1, 0.1));
  defaultShader->setVec3f("dirLight.diffuse", glm::vec3(0.1, 0.1, 0.1));
  defaultShader->setVec3f("dirLight.specular", glm::vec3(0.2, 0.2, 0.2));

  defaultShader->setVec3f("spotLight.position",  camera.getPosition());
  defaultShader->setVec3f("spotLight.direction", camera.getFront());
  defaultShader->setFloat("spotLight.cutOff",   glm::cos(glm::radians(12.5f)));
  defaultShader->setFloat("spotLight.outerCutOff",   glm::cos(glm::radians(17.5f)));
  defaultShader->setVec3f("spotLight.ambient", glm::vec3(0.2, 0.2, 0.2));
  defaultShader->setVec3f("spotLight.diffuse", glm::vec3(0.5, 0.5, 0.5));
  defaultShader->setVec3f("spotLight.specular", glm::vec3(1, 1, 1));
  int i = 0;
  for (auto info : lightInfos) {
    defaultShader->setFloat("pointLights["+std::to_string(i)+"].constant", info.constant);
    defaultShader->setFloat("pointLights["+std::to_string(i)+"].linear", info.linear);
    defaultShader->setFloat("pointLights["+std::to_string(i)+"].quadratic", info.quadratic);
    defaultShader->setVec3f("pointLights["+std::to_string(i)+"].position", info.position);
    defaultShader->setVec3f("pointLights["+std::to_string(i)+"].ambient", info.ambient);
    defaultShader->setVec3f("pointLights["+std::to_string(i)+"].diffuse", info.diffuse);
    defaultShader->setVec3f("pointLights["+std::to_string(i)+"].specular", info.specular);
    i++;
  }

  defaultShader->setVec3f("viewPos", camera.getPosition());
  defaultShader->setMatrix4f("view", view);
  for (auto info : modelInfos) {
    glm::mat4 modelMat = getModelMat(info);
    defaultShader->setMatrix4f("model", modelMat);
    info.model->draw(*defaultShader);
  }
}

void Map::tick(float time) {}

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

LightInfo Map::parseLightInfo(nlohmann::basic_json<> jLight) {
  LightInfo info;
  std::vector<float> position = jLight["position"];
  info.position = glm::vec3(position[0], position[1], position[2]);
  float constant = jLight["constant"];
  info.constant = constant;
  float linear = jLight["linear"];
  info.linear = linear;
  float quadratic = jLight["quadratic"];
  info.quadratic = quadratic;
  std::vector<float> ambient = jLight["ambient"];
  info.ambient = glm::vec3(ambient[0], ambient[1], ambient[2]);
  std::vector<float> diffuse = jLight["diffuse"];
  info.diffuse = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
  std::vector<float> specular = jLight["specular"];
  info.specular = glm::vec3(specular[0], specular[1], specular[2]);
  std::string modelPath = "./resources/models/white_cube.json";
  Model *model = new Model(modelPath.c_str());
  info.model = model;
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