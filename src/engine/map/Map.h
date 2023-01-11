#ifndef MAP_H
#define MAP_H

#include "../model/Model.h"

class Map {
public:
    explicit Map(const char *mapModelPath);

    glm::vec3 getSpawnPos();

    Model &getModel();

private:

    Model model;

    const glm::vec3 *getSpawnPos(const Node &node, glm::vec3 &translation);
};

#endif