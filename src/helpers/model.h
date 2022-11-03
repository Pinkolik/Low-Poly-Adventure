#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
#include "texture.h"
#include <string>
#include <vector>

class Model {
public:
  Model(const char *modelPath, float scaleTexX, float scaleTexY);
  Model(const char *modelPath);
  void draw(Shader &shader);

private:
  unsigned int VAO, VBO, verticesSize;
  Texture *texture;
  std::vector<float> color;
  bool hasTexture;
  bool hasColor;
};

#endif