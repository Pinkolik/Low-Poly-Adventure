#ifndef UTILS_H
#define UTILS_H
#include "node.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class Utils {
public:
  static vector<Node> debugNodes;
  static float getMinFloat(vector<float> floats);
  static bool isPointInsideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                                    glm::vec3 normal, glm::vec3 point);

  static Node createBoxNode();
};
#endif