#ifndef MAP_H
#define MAP_H
#include "model.h"

class Map {
public:
  Map(Model &mapModel);

  Model &getMapModel();

private:
  Model mapModel;

  glm::vec3 getSpawnPos();
};

#endif