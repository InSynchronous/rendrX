#include "Block.h"
#include <utility>

namespace rendrx {
Block::Block(glm::vec3 pos, std::pair<glm::vec2, glm::vec2> uv)
    : pos(pos), air(false) {
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
std::unique_ptr<Quad> Block::getFront() const {
    return std::make_unique<Quad>(*front);
}

std::unique_ptr<Quad> Block::getBack() const {
    return std::make_unique<Quad>(*back);
}

std::unique_ptr<Quad> Block::getTop() const {
    return std::make_unique<Quad>(*top);
}

std::unique_ptr<Quad> Block::getBottom() const {
    return std::make_unique<Quad>(*bottom);
}

std::unique_ptr<Quad> Block::getRight() const {
    return std::make_unique<Quad>(*right);
}

std::unique_ptr<Quad> Block::getLeft() const {
    return std::make_unique<Quad>(*left);
}

} // namespace rendrx
