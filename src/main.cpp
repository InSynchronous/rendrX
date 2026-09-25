#include "rendrx/Atlas.h"
#include "rendrx/Quad.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;
    float s = 0.5f;

    // Front (+Z)
    auto uv = rendrx::getUV(3, 15);
    float u0 = uv.first.x;
    float u1 = uv.second.x;
    float v0 = uv.first.y;
    float v1 = uv.second.y;

    scene.addObject(std::make_unique<rendrx::Quad>(
        glm::vec3{-s, -s, +s}, glm::vec3{+s, -s, +s}, glm::vec3{+s, +s, +s},
        glm::vec3{-s, +s, +s}, uv));

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
