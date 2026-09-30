#include "Generation.h"
#include "Noise.h"
#include <algorithm>

using namespace rendrx;

float Generation::getHeight(glm::vec2 xy) {
    float n = noise.fbm(xy * 0.015f, // terrain scale
                        5,           // octaves
                        1.0f,        // amplitude
                        1.0f,        // frequency
                        2.0f,        // lacunarity
                        0.5f         // persistence
    );

    int height = 80 + static_cast<int>(n * 35.0f);

    return std::clamp(height, 1, 255);
}

Block Generation::getBlock(glm::vec3 xyz) {
    auto h = getHeight({xyz.x, xyz.z});
    auto y = xyz.y;

    if (y == h)
        return Block::GRASS;
    if (y > 80)
        return Block::DIRT;
    else
        return Block::STONE;
}

Block Generation::getBlock(float y, float h) {
    if (y == h)
        return Block::GRASS;
    if (y > 80)
        return Block::DIRT;
    else
        return Block::STONE;
}
