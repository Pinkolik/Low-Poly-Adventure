#ifndef MESH_H
#define MESH_H

#include "Primitive.h"
#include <vector>

class Mesh {
public:
    Mesh(int id, std::vector<Primitive> &primitives);

    const std::vector<Primitive> &getPrimitives() const;

    int getId() const;

    void applyTranslation(glm::mat4 transMat);

    void applyTranslationToAABBs(glm::mat4 transMat);

    void calculateAABBs();

private:

    const int id;
    std::vector<Primitive> primitives;

    void buffer();

};

#endif
