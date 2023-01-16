#ifndef PROJECTION_H
#define PROJECTION_H

#include <cmath>
#include <glm/vec3.hpp>
#include <vector>

class Projection {
public:
    Projection(glm::vec3 &axis, glm::vec3 *triangle);

    bool isIntersecting(Projection &other) const;

    float findIntersectionLength(Projection &other) const;

private:
    float min = INFINITY;
    float max = -INFINITY;
};

#endif