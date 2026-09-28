#pragma once

#include <array>
#include <cstddef>
#include <glm/glm.hpp>

namespace rendrx {
class Noise {
  private:
    size_t seed;
    std::array<int, 512> perm;

    static float fade(float t);
    static float lerp(float a, float b, float t);
    static float gradient(int hash, glm::vec2 vec);

  public:
    Noise(size_t seed);

    float perlin(glm::vec2 vec);
    float fbm(glm::vec2 position, int octaves, float frequency, float amplitude,
              float lacunarity, float persistence);
};

} // namespace rendrx
