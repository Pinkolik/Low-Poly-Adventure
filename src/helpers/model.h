#ifndef MODEL_H
#define MODEL_H
#include "texture.h"
#include <string>
#include <vector>

class Model {
public:
  Model(const char *modelPath, float scaleTexX, float scaleTexY);
  Model(const char *modelPath);
  void draw();

private:
  unsigned int VAO, VBO, verticesSize;
  Texture *texture;
};

#endif