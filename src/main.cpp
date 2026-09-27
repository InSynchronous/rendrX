#include "rendrx/Scene.h"
#include "rendrx/World.h"
#include <iostream>
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    World world(0);
    world.loadChunks({0, 0}, 5);

    world.generateMeshes(); // preload 5 chunks

    scene.addWorld(world);

    scene.launch();

    while (!scene.shouldClose()) {
        glm::vec3 prevPos = scene.getPrevCameraPos();
        glm::vec3 pos = scene.getCameraPos();

        glm::ivec2 prevChunkPos = {
            static_cast<int>(std::floor(prevPos.x / 16.0f)),
            static_cast<int>(std::floor(prevPos.z / 16.0f)),
        };
        glm::ivec2 chunkPos = {
            static_cast<int>(std::floor(pos.x / 16.0f)),
            static_cast<int>(std::floor(pos.z / 16.0f)),
        };

        if (prevChunkPos != chunkPos) {
            world.loadChunks(chunkPos, 5);
            world.unloadChunks(chunkPos, 5);
            world.generateMeshes();
        }

        scene.render();
    }

    return 0;
}
