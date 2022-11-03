#include "map.h"
#include "glm/fwd.hpp"
#include "model.h"
#include "shader.h"
#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

Map::Map(const char *mapPath) {
  std::ifstream jFile(mapPath);
  json jObj = json::parse(jFile);

  std::map<std::string, Model *> nameToModel;
  std::vector<ModelInfo> infos;
  for (auto jModel : jObj["models"]) {
    ModelInfo info;
    std::string modelName = std::string(jModel["model"]);
    if (nameToModel.find(modelName) == nameToModel.end()) {
      std::string modelPath = "./resources/models/" + modelName + ".json";
      Model *model = new Model(modelPath.c_str());
      info.model = model;
      nameToModel[modelName] = model;
    } else {
      info.model = nameToModel[modelName];
    }
    std::vector<float> scale = jModel["scale"];
    info.scale = scale;
    std::vector<float> translate = jModel["translate"];
    info.translate = translate;
    std::vector<float> rotate = jModel["rotate"];
    info.rotate = rotate;
    modelInfos.push_back(info);
  }
}

void Map::draw(Shader *shader) {

  for (auto info : modelInfos) {
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
    modelMat = glm::scale(
        modelMat, glm::vec3(info.scale[0], info.scale[1], info.scale[2]));
    shader->setMatrix4f("model", modelMat);
    info.model->draw();
  }
}