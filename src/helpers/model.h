#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
class Model {
public:
  Model(const char *path);
  void draw(Shader &shader);
};

#endif