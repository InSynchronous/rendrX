#pragma once

#include "Object.h"
#include "rendrx/Atlas.h"
#include <glm/glm.hpp>
#include <utility>
#include <vector>

namespace rendrx {

class Quad : public Object {
  public:
    Quad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, UV uv)
        : p1(p1), p2(p2), p3(p3), p4(p4), u1(uv.min.x, uv.min.y),
          u2(uv.max.x, uv.min.y), u3(uv.max.x, uv.max.y),
          u4(uv.min.x, uv.max.y) {}

    Quad(glm::vec3 p1, glm::vec2 u1, glm::vec3 p2, glm::vec2 u2, glm::vec3 p3,
         glm::vec2 u3, glm::vec3 p4, glm::vec2 u4)
        : p1(p1), u1(u1), p2(p2), u2(u2), p3(p3), u3(u3), p4(p4), u4(u4) {}

    glm::vec3 p1, p2, p3, p4;
    glm::vec2 u1, u2, u3, u4;

    const std::vector<float> flatten() override;
};

} // namespace rendrx
