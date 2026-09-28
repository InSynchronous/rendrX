#include "Block.h"
#include <utility>

namespace rendrx {
std::unique_ptr<Quad> Block::getFront() {
    if (!front) {
        float s = 0.5f;

        front = std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, +s}, pos + glm::vec3{+s, -s, +s},
            pos + glm::vec3{+s, +s, +s}, pos + glm::vec3{-s, +s, +s}, uv);
    }

    return std::move(front);
}
std::unique_ptr<Quad> Block::getBack() {
    if (!back) {
        float s = 0.5f;

        back = std::make_unique<Quad>(
            pos + glm::vec3{+s, -s, -s}, pos + glm::vec3{-s, -s, -s},
            pos + glm::vec3{-s, +s, -s}, pos + glm::vec3{+s, +s, -s}, uv);
    }

    return std::move(back);
}

std::unique_ptr<Quad> Block::getTop() {
    if (!top) {
        float s = 0.5f;

        top = std::make_unique<Quad>(
            pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{+s, +s, +s},
            pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{-s, +s, -s}, uv);
    }

    return std::move(top);
}

std::unique_ptr<Quad> Block::getBottom() {
    if (!bottom) {
        float s = 0.5f;

        bottom = std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{+s, -s, -s},
            pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{-s, -s, +s}, uv);
    }

    return std::move(bottom);
}

std::unique_ptr<Quad> Block::getRight() {
    if (!right) {
        float s = 0.5f;

        right = std::make_unique<Quad>(
            pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{+s, -s, -s},
            pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{+s, +s, +s}, uv);
    }

    return std::move(right);
}

std::unique_ptr<Quad> Block::getLeft() {
    if (!left) {
        float s = 0.5f;

        left = std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{-s, -s, +s},
            pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{-s, +s, -s}, uv);
    }

    return std::move(left);
}
} // namespace rendrx
