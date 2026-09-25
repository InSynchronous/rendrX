#pragma once

#include "rendrx/Quad.h"
#include <glm/glm.hpp>
#include <memory>

namespace rendrx {
class Block {
  private:
    std::unique_ptr<Quad> front, back, top, bottom, right, left;

  public:
    Block(glm::vec3 pos, std::pair<glm::vec2, glm::vec2> uv);

    std::unique_ptr<Quad> getFront();
    std::unique_ptr<Quad> getBack();
    std::unique_ptr<Quad> getTop();
    std::unique_ptr<Quad> getBottom();
    std::unique_ptr<Quad> getRight();
    std::unique_ptr<Quad> getLeft();

    glm::vec3 pos;
};
} // namespace rendrx
