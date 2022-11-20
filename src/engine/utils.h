#ifndef UTILS_H
#define UTILS_H
#include "node.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class Utils {
public:
  static glm::vec3 *getMinDistanceToOriginVector(vector<glm::vec3 *> &points,
                                                 glm::vec3 origin);
  static bool isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                                    glm::vec3 normal, glm::vec3 point);

  static Node createBoxNode();
};
#endif