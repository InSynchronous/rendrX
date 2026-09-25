#include "rendrx/Atlas.h"
#include "rendrx/Block.h"
#include "rendrx/Scene.h"
using namespace rendrx;

int main() {
    rendrx::Scene scene;

    auto uv = rendrx::getUV(3, 15);
    Block block({0, 0, 0}, uv); // block at origin

    scene.addObject(block.getFront());
    scene.addObject(block.getBack());
    scene.addObject(block.getLeft());
    scene.addObject(block.getRight());
    scene.addObject(block.getTop());
    scene.addObject(block.getBottom());

    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
