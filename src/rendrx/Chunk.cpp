#include "Chunk.h"
#include "Atlas.h"
using namespace rendrx;

Chunk::Chunk() {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            blocks[x][0][z] = Block(glm::vec3{x, 0, z}, getUV(0, 0));
        }
    }
}

Block &Chunk::getBlock(int x, int y, int z) { return blocks[x][y][z]; }
