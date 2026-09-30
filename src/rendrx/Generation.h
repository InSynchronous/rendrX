#pragma once

#include "Noise.h"
#include "rendrx/Block.h"
#include <glm/fwd.hpp>
namespace rendrx {
class Generation {
  private:
    Noise noise;

  public:
    Generation(int seed) : noise(seed) {}
    float getHeight(glm::vec2 xz);
    Block getBlock(glm::vec3 xyz);
    Block getBlock(float y, float height);
};
} // namespace rendrx
