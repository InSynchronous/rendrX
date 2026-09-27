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

            auto it = chunks.find(chunkPos);
            if (it == chunks.end()) {
                // only generate if not alr known
                chunks[chunkPos] = std::make_unique<Chunk>(chunkPos);
            }
        }
    }
}

void World::unloadChunks(glm::ivec2 position, size_t radius) {
    int rSquared = static_cast<int>(radius * radius);

    for (auto it = chunks.begin(); it != chunks.end();) {
        glm::ivec2 delta = it->first - position;

        if (delta.x * delta.x + delta.y * delta.y > rSquared) {
            it = chunks.erase(it);
        } else {
            ++it;
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

void World::generateMeshes() {
    for (auto &[position, chunk] : chunks) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    glm::vec3 worldPos{position.x * 16 + i, j,
                                       position.y * 16 + k};

                    Block &b = getBlock(worldPos);

                    if (b.isAir()) {
                        continue;
                    }

                    if (getBlock(worldPos + glm::vec3{0, 0, -1}).isAir())
                        chunk->addObject(b.getBack());

                    if (getBlock(worldPos + glm::vec3{0, 0, 1}).isAir())
                        chunk->addObject(b.getFront());

                    if (getBlock(worldPos + glm::vec3{0, 1, 0}).isAir())
                        chunk->addObject(b.getTop());

                    if (getBlock(worldPos + glm::vec3{0, -1, 0}).isAir())
                        chunk->addObject(b.getBottom());

                    if (getBlock(worldPos + glm::vec3{-1, 0, 0}).isAir())
                        chunk->addObject(b.getLeft());

                    if (getBlock(worldPos + glm::vec3{1, 0, 0}).isAir())
                        chunk->addObject(b.getRight());
                }
            }
        }

        // compile to vertecies and push to gpu
        chunk->uploadTriangles();
    }
}
