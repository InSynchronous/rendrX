#include "rendrx/Scene.h"
#include "rendrx/World.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    World world(0);
    world.loadChunks({0, 0}, 5);

    world.generateMeshes(); // preload 5 chunks

    scene.addWorld(world);

    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
