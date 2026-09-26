#include "rendrx/Chunk.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    Chunk chunk;
    std::cout << chunk.getBlock(16, 0, 0).isAir() << '\n';
    std::cout << chunk.getBlock(-1, 0, 0).isAir() << '\n';
    std::cout << chunk.getBlock(0, 16, 0).isAir() << '\n';
    for (auto i = 0; i < 16; i++) {
        for (auto j = 0; j < 16; j++) {
            for (auto k = 0; k < 16; k++) {
                Block &b = chunk.getBlock(i, j, k);
                if (b.isAir()) {
                    continue;
                }
                if (chunk.getBlock(i, j, k - 1).isAir())
                    scene.addObject(b.getBack());
                if (chunk.getBlock(i, j, k + 1).isAir())
                    scene.addObject(b.getFront());
                if (chunk.getBlock(i, j + 1, k).isAir())
                    scene.addObject(b.getTop());
                if (chunk.getBlock(i, j - 1, k).isAir())
                    scene.addObject(b.getBottom());
                if (chunk.getBlock(i - 1, j, k).isAir())
                    scene.addObject(b.getLeft());
                if (chunk.getBlock(i + 1, j, k).isAir())
                    scene.addObject(b.getRight());
            }
        }
    }

    scene.launch();

    while (!scene.shouldClose()) {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        scene.render();
    }

    return 0;
}
