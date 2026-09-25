#include "Atlas.h"
#include <utility>

std::pair<glm::vec2, glm::vec2> rendrx::getUV(int x, int y) {
    float u0 = x * 16.0f / 256.0f;
    float v0 = y * 16.0f / 256.0f;
    float u1 = (x + 1) * 16.0f / 256.0f;
    float v1 = (y + 1) * 16.0f / 256.0f;

    return {glm::vec2{u0, v0}, glm::vec2{u1, v1}};
}
