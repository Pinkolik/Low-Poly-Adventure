#ifndef PROJECTION_H
#define PROJECTION_H
#include <cmath>
#include <glm/vec3.hpp>
#include <vector>

class Projection {
public:
  Projection(glm::vec3 axis, std::vector<glm::vec3> &triangle);
  float getMin();
  float getMax();
  bool isIntersecting(Projection &other);

private:
  float min = INFINITY;
  float max = -INFINITY;
};

#endif