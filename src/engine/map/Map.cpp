#include "Map.h"

Map::Map(const char *mapModelPath) :
        model(Model(mapModelPath)) {
    model.buffer();
}

Model &Map::getModel() {
    return model;
}

glm::vec3 Map::getSpawnPos() {
    return model.getSpawnPos();
}
