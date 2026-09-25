#include "rendrx/Quad.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;
    float s = 0.5f;

    // Front (+Z)
    constexpr float x = 4;
    constexpr float y = 15;

    constexpr float u0 = x * 16.0f / 256.0f;
    constexpr float v0 = y * 16.0f / 256.0f;
    constexpr float u1 = (x + 1) * 16.0f / 256.0f;
    constexpr float v1 = (y + 1) * 16.0f / 256.0f;

    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, +s}, glm::vec2{u0, v0}, glm::vec3{+s, -s, +s},
        glm::vec2{u1, v0}, glm::vec3{+s, +s, +s}, glm::vec2{u1, v1},
        glm::vec3{-s, +s, +s}, glm::vec2{u0, v1}));

    // Back (-Z)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{+s, -s, -s}, glm::vec2{u0, v0}, glm::vec3{-s, -s, -s},
        glm::vec2{u1, v0}, glm::vec3{-s, +s, -s}, glm::vec2{u1, v1},
        glm::vec3{+s, +s, -s}, glm::vec2{u0, v1}));

    // Right (+X)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{+s, -s, +s}, glm::vec2{u0, v0}, glm::vec3{+s, -s, -s},
        glm::vec2{u1, v0}, glm::vec3{+s, +s, -s}, glm::vec2{u1, v1},
        glm::vec3{+s, +s, +s}, glm::vec2{u0, v1}));

    // Left (-X)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, -s}, glm::vec2{u0, v0}, glm::vec3{-s, -s, +s},
        glm::vec2{u1, v0}, glm::vec3{-s, +s, +s}, glm::vec2{u1, v1},
        glm::vec3{-s, +s, -s}, glm::vec2{u0, v1}));

    // Top (+Y)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, +s, +s}, glm::vec2{u0, v0}, glm::vec3{+s, +s, +s},
        glm::vec2{u1, v0}, glm::vec3{+s, +s, -s}, glm::vec2{u1, v1},
        glm::vec3{-s, +s, -s}, glm::vec2{u0, v1}));

    // Bottom (-Y)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, -s}, glm::vec2{u0, v0}, glm::vec3{+s, -s, -s},
        glm::vec2{u1, v0}, glm::vec3{+s, -s, +s}, glm::vec2{u1, v1},
        glm::vec3{-s, -s, +s}, glm::vec2{u0, v1}));
    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
