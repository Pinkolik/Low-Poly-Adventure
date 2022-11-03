#include "model.h"
#include "texture.h"
#include <cstddef>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

Model::Model(const char *modelPath) : Model(modelPath, 1.0f, 1.0f) {}

Model::Model(const char *modelPath, float scaleTexX, float scaleTexY) {
  std::ifstream jFile(modelPath);
  json jObj = json::parse(jFile);

  std::vector<float> jVertices = jObj["vertices"];

  hasTexture = !jObj["texture"].is_null();
  if (hasTexture) {
    std::string texPath =
        "./resources/textures/" + std::string(jObj["texture"]);
    Texture *jTexture = new Texture(texPath.c_str());
    texture = jTexture;
    int i = 0;
    for (auto it = jVertices.begin(); it != jVertices.end(); it++) {
      if (i == 3) {
        *it *= scaleTexX;
      } else if (i == 4) {
        *it *= scaleTexY;
      }
      i = (i + 1) % 5;
    }
  }
  hasColor = !jObj["color"].is_null();
  if (hasColor) {
    std::vector<float> jColor = jObj["color"];
    color = jColor;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, jVertices.size() * sizeof(float),
               jVertices.data(), GL_STATIC_DRAW);

  if (hasTexture) {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    verticesSize = jVertices.size() / 5;
  } else {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    verticesSize = jVertices.size() / 3;
  }
}

void Model::draw(Shader &shader) {
  if (hasTexture) {
    shader.setBool("hasColor", false);
    texture->active();
  } else if (hasColor) {
    shader.setBool("hasColor", true);
    shader.setVec3f("color", glm::vec3(color[0], color[1], color[2]));
  }
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, verticesSize);
}