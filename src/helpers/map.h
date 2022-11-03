#ifndef MAP_H
#define MAP_H
#include "model.h"
#include "shader.h"
#include <vector>

struct ModelInfo {
  Model *model;
  std::vector<float> scale;
  std::vector<float> translate;
  std::vector<float> rotate;
};

class Map {
public:
  Map(const char *mapPath);
  void draw(Shader *shader);

private:
  std::vector<ModelInfo> modelInfos;
};

#endif