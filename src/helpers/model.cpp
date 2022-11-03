#include "model.h"
#include "texture.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

Model::Model(std::string modelPath) { Model(modelPath.c_str()); }

Model::Model(const char *modelPath) {
  std::ifstream jFile(modelPath);
  json jObj = json::parse(jFile);

  std::string texPath = "./resources/textures/" + std::string(jObj["texture"]);
  Texture *jTexture = new Texture(texPath.c_str());
  texture = jTexture;
  std::vector<float> jVertices = jObj["vertices"];

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