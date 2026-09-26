#include "Chunk.h"
#include "Atlas.h"
using namespace rendrx;

Chunk::Chunk() {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            blocks[x][0][z] = Block(glm::vec3{x, 0, z}, getUV(2, 15));
        }
    }
}

Block &Chunk::getBlock(int x, int y, int z) {
    static Block airBlock;

    if (x < 0 || x >= 16 || y < 0 || y >= 16 || z < 0 || z >= 16) {
        return airBlock;
    }

    return blocks[x][y][z];
}
