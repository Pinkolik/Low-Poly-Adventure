#ifndef PROJECTION_H
#define PROJECTION_H

#include <cmath>
#include <glm/vec3.hpp>
#include <vector>

class Projection {
public:
    Projection(glm::vec3 &axis, std::vector<glm::vec3> &triangle);

    bool isIntersecting(Projection &other);

    float findIntersectionLength(Projection &other);

private:
    float min = INFINITY;
    float max = -INFINITY;
};

#endif