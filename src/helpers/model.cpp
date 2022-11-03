#include "model.h"
#include "texture.h"
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

  std::string texPath = "./resources/textures/" + std::string(jObj["texture"]);
  Texture *jTexture = new Texture(texPath.c_str());
  texture = jTexture;
  std::vector<float> jVertices = jObj["vertices"];

  int i = 0;
  for (auto it = jVertices.begin(); it != jVertices.end(); it++) {
    if (i == 3) {
      *it *= scaleTexX;
    } else if (i == 4) {
      *it *= scaleTexY;
    }
    i = (i + 1) % 5;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, jVertices.size() * sizeof(float),
               jVertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  verticesSize = jVertices.size() / 5;
}

void Model::draw() {
  texture->active();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, verticesSize);
}