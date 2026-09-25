#include "rendrx/Quad.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;
    float s = 0.5f;

    // Front (+Z)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, +s}, glm::vec2{0.0f, 0.0f}, glm::vec3{+s, -s, +s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{+s, +s, +s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{-s, +s, +s}, glm::vec2{0.0f, 1.0f}));

    // Back (-Z)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{+s, -s, -s}, glm::vec2{0.0f, 0.0f}, glm::vec3{-s, -s, -s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{-s, +s, -s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{+s, +s, -s}, glm::vec2{0.0f, 1.0f}));

    // Right (+X)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{+s, -s, +s}, glm::vec2{0.0f, 0.0f}, glm::vec3{+s, -s, -s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{+s, +s, -s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{+s, +s, +s}, glm::vec2{0.0f, 1.0f}));

    // Left (-X)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, -s}, glm::vec2{0.0f, 0.0f}, glm::vec3{-s, -s, +s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{-s, +s, +s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{-s, +s, -s}, glm::vec2{0.0f, 1.0f}));

    // Top (+Y)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, +s, +s}, glm::vec2{0.0f, 0.0f}, glm::vec3{+s, +s, +s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{+s, +s, -s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{-s, +s, -s}, glm::vec2{0.0f, 1.0f}));

    // Bottom (-Y)
    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, -s}, glm::vec2{0.0f, 0.0f}, glm::vec3{+s, -s, -s},
        glm::vec2{1.0f, 0.0f}, glm::vec3{+s, -s, +s}, glm::vec2{1.0f, 1.0f},
        glm::vec3{-s, -s, +s}, glm::vec2{0.0f, 1.0f}));
    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
