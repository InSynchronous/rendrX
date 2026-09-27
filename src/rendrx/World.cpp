#include "World.h"
#include "rendrx/Chunk.h"
#include <memory>

using namespace rendrx;

void World::loadChunks(glm::ivec2 position, size_t radius) {
    int rSquared = radius * radius;

    for (int x = -static_cast<int>(radius); x <= static_cast<int>(radius);
         ++x) {
        int maxY = static_cast<int>(std::sqrt(rSquared - x * x));

        for (int y = -maxY; y <= maxY; ++y) {
            glm::ivec2 chunkPos = position + glm::ivec2{x, y};

            chunks[chunkPos] = std::make_unique<Chunk>(chunkPos);
        }
    }
}

Block &World::getBlock(glm::vec3 position) {
    static Block air;

    int chunkX = static_cast<int>(std::floor(position.x / 16.0f));
    int chunkZ = static_cast<int>(std::floor(position.z / 16.0f));

    int localX = static_cast<int>(std::floor(position.x)) - chunkX * 16;
    int localZ = static_cast<int>(std::floor(position.z)) - chunkZ * 16;

    auto it = chunks.find({chunkX, chunkZ});

    if (it == chunks.end()) {
        return air;
    }

    return it->second->getBlock(
        {localX, static_cast<int>(std::floor(position.y)), localZ});
}
