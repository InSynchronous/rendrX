#include "rendrx/Chunk.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    Chunk chunk;
    for (auto i = 0; i < 16; i++) {
        for (auto j = 0; j < 16; j++) {
            for (auto k = 0; k < 16; k++) {
                Block &b = chunk.getBlock(i, j, k);
                if (b.isAir()) {
                    continue;
                }
                scene.addObject(b.getBack());
                scene.addObject(b.getFront());
                scene.addObject(b.getTop());
                scene.addObject(b.getBottom());
                scene.addObject(b.getLeft());
                scene.addObject(b.getRight());
            }
        }
    }

    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
