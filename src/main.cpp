#include "rendrx/Scene.h"
#include "rendrx/World.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    World world(0);
    world.loadChunks({0, 0}, 5);

    for (auto &[position, chunk] : world.chunks) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    glm::vec3 worldPos{position.x * 16 + i, j,
                                       position.y * 16 + k};

                    Block &b = world.getBlock(worldPos);

                    if (b.isAir()) {
                        continue;
                    }

                    if (world.getBlock(worldPos + glm::vec3{0, 0, -1}).isAir())
                        scene.addObject(b.getBack());

                    if (world.getBlock(worldPos + glm::vec3{0, 0, 1}).isAir())
                        scene.addObject(b.getFront());

                    if (world.getBlock(worldPos + glm::vec3{0, 1, 0}).isAir())
                        scene.addObject(b.getTop());

                    if (world.getBlock(worldPos + glm::vec3{0, -1, 0}).isAir())
                        scene.addObject(b.getBottom());

                    if (world.getBlock(worldPos + glm::vec3{-1, 0, 0}).isAir())
                        scene.addObject(b.getLeft());

                    if (world.getBlock(worldPos + glm::vec3{1, 0, 0}).isAir())
                        scene.addObject(b.getRight());
                }
            }
        }
    }

    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
