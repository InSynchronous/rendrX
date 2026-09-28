#pragma once

#include <array>
#include <cstddef>
#include <glm/glm.hpp>

namespace rendrx {
class Noise {
  private:
    size_t seed;
    static float fade(float t);
    static float lerp(float a, float b, float t);
    static float gradient(int hash, glm::vec2 vec);

    std::array<int, 512> perm;

  public:
    Noise(size_t seed);
    float perlin(glm::vec2 vec);
};
} // namespace rendrx
