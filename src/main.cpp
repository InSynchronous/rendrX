#include "rendrx/Scene.h"
#include "rendrx/Triangle.h"

int main() {
    rendrx::Scene scene;
    rendrx::Triangle tri = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f},
                            {0.5f, -0.5f, 0.0f},  {1.0f, 0.0f},
                            {0.0f, 0.5f, 0.0f},   {0.5f, 1.0f}};
    scene.addTriangle(tri);
    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
