#include "Block.h"
#include <utility>

namespace rendrx {
Block::Block(glm::vec3 pos, std::pair<glm::vec2, glm::vec2> uv) : pos(pos) {
    float s = 0.5f;

    // Front (+Z)
    front = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{-s, -s, +s}, pos + glm::vec3{+s, -s, +s},
        pos + glm::vec3{+s, +s, +s}, pos + glm::vec3{-s, +s, +s}, uv);

    // Back (-Z)
    back = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{+s, -s, -s}, pos + glm::vec3{-s, -s, -s},
        pos + glm::vec3{-s, +s, -s}, pos + glm::vec3{+s, +s, -s}, uv);

    // Top (+Y)
    top = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{+s, +s, +s},
        pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{-s, +s, -s}, uv);

    // Bottom (-Y)
    bottom = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{+s, -s, -s},
        pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{-s, -s, +s}, uv);

    // Right (+X)
    right = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{+s, -s, -s},
        pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{+s, +s, +s}, uv);

    // Left (-X)
    left = std::make_unique<rendrx::Quad>(
        pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{-s, -s, +s},
        pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{-s, +s, -s}, uv);
}

// Getters
std::unique_ptr<Quad> Block::getFront() { return std::move(front); }
std::unique_ptr<Quad> Block::getBack() { return std::move(back); }
std::unique_ptr<Quad> Block::getTop() { return std::move(top); }
std::unique_ptr<Quad> Block::getBottom() { return std::move(bottom); }
std::unique_ptr<Quad> Block::getRight() { return std::move(right); }
std::unique_ptr<Quad> Block::getLeft() { return std::move(left); }

} // namespace rendrx
