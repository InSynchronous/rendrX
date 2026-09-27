#include "Chunk.h"
#include "Atlas.h"
using namespace rendrx;

Chunk::Chunk(glm::ivec2 chunkCoord) : chunkCoord(chunkCoord) {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            blocks[x][0][z] = Block(
                glm::vec3{x + chunkCoord.x * 16, 0, z + chunkCoord.y * 16},
                getUV(2, 15));
        }
    }
}

Block &Chunk::getBlock(glm::vec3 position) {
    static Block airBlock;

    // refactor this slop bro
    int x = position.x;
    int y = position.y;
    int z = position.z;

    if (x < 0 || x >= 16 || y < 0 || y >= 16 || z < 0 || z >= 16) {
        return airBlock;
    }

    return blocks[x][y][z];
}
