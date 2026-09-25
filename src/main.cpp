#include "rendrx/Scene.h"
#include "rendrx/Triangle.h"

int main() {
    rendrx::Scene scene;
    // Front
    scene.addTriangle({{-0.5f, -0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, -0.5f, 0.5f},
                       {1.0f, 0.0f},
                       {0.5f, 0.5f, 0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{-0.5f, -0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, 0.5f, 0.5f},
                       {1.0f, 1.0f},
                       {-0.5f, 0.5f, 0.5f},
                       {0.0f, 1.0f}});

    // Back
    scene.addTriangle({{0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {-0.5f, -0.5f, -0.5f},
                       {1.0f, 0.0f},
                       {-0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {-0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f},
                       {0.5f, 0.5f, -0.5f},
                       {0.0f, 1.0f}});

    // Left
    scene.addTriangle({{-0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {-0.5f, -0.5f, 0.5f},
                       {1.0f, 0.0f},
                       {-0.5f, 0.5f, 0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{-0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {-0.5f, 0.5f, 0.5f},
                       {1.0f, 1.0f},
                       {-0.5f, 0.5f, -0.5f},
                       {0.0f, 1.0f}});

    // Right
    scene.addTriangle({{0.5f, -0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, -0.5f, -0.5f},
                       {1.0f, 0.0f},
                       {0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{0.5f, -0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f},
                       {0.5f, 0.5f, 0.5f},
                       {0.0f, 1.0f}});

    // Top
    scene.addTriangle({{-0.5f, 0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, 0.5f, 0.5f},
                       {1.0f, 0.0f},
                       {0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{-0.5f, 0.5f, 0.5f},
                       {0.0f, 0.0f},
                       {0.5f, 0.5f, -0.5f},
                       {1.0f, 1.0f},
                       {-0.5f, 0.5f, -0.5f},
                       {0.0f, 1.0f}});

    // Bottom
    scene.addTriangle({{-0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {0.5f, -0.5f, -0.5f},
                       {1.0f, 0.0f},
                       {0.5f, -0.5f, 0.5f},
                       {1.0f, 1.0f}});

    scene.addTriangle({{-0.5f, -0.5f, -0.5f},
                       {0.0f, 0.0f},
                       {0.5f, -0.5f, 0.5f},
                       {1.0f, 1.0f},
                       {-0.5f, -0.5f, 0.5f},
                       {0.0f, 1.0f}});
    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
