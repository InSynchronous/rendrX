#pragma once

#include <glm/glm.hpp>
#include <utility>

namespace rendrx {

struct ivec2 {
    uint8_t x;
    uint8_t y;
};
struct vec2 {
    float x;
    float y;
};
struct UV {
    vec2 min;
    vec2 max;
};

constexpr UV getUV(ivec2 atlasCoordinate) {
    auto x = atlasCoordinate.x;
    auto y = atlasCoordinate.y;
    float u0 = x * 16.0f / 256.0f;
    float v0 = y * 16.0f / 256.0f;
    float u1 = (x + 1) * 16.0f / 256.0f;
    float v1 = (y + 1) * 16.0f / 256.0f;

    return {u0, v0, u1, v1};
}
} // namespace rendrx
