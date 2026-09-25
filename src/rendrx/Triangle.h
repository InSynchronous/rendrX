#pragma once

#include "Object.h"
#include <glm/glm.hpp>
#include <vector>

namespace rendrx {

class Triangle : public Object {
  public:
    Triangle(glm::vec3 p1, glm::vec2 u1, glm::vec3 p2, glm::vec2 u2,
             glm::vec3 p3, glm::vec2 u3)
        : p1(p1), u1(u1), p2(p2), u2(u2), p3(p3), u3(u3) {}

    glm::vec3 p1, p2, p3;
    glm::vec2 u1, u2, u3;

    const std::vector<float> flatten() override;
};

} // namespace rendrx
