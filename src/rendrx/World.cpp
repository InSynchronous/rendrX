#include "World.h"
#include "rendrx/Atlas.h"
#include "rendrx/Block.h"
#include "rendrx/Chunk.h"
#include "rendrx/Quad.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>

using namespace rendrx;

void World::loadChunks(glm::ivec2 position, size_t radius) {
    int rSquared = radius * radius;
    std::lock_guard lock(queueMutex);

    {
        for (int x = -static_cast<int>(radius); x <= static_cast<int>(radius);
             ++x) {
            int maxY = static_cast<int>(std::sqrt(rSquared - x * x));

            for (int y = -maxY; y <= maxY; ++y) {
                glm::ivec2 chunkPos = position + glm::ivec2{x, y};

                auto it = chunks.find(chunkPos);
                if (it == chunks.end() &&
                    chunksQueued.find(chunkPos) == chunksQueued.end()) {

                    chunksToLoad.push(chunkPos);
                    chunksQueued.insert(chunkPos);
                }
            }
        }
    }
    queueCV.notify_one();
}

void World::generationLoop() {
    while (true) {
        glm::ivec2 pos;

        {
            std::unique_lock lock(queueMutex);

            queueCV.wait(lock,
                         [this] { return !chunksToLoad.empty() || !running; });

            if (!running && chunksToLoad.empty())
                return;

            pos = chunksToLoad.front();
            chunksToLoad.pop();
        }

        auto start = std::chrono::high_resolution_clock::now();

        auto temp = std::make_unique<Chunk>(pos, generation);

        generateMeshCPU(temp);

        {
            std::unique_lock lock(queueMutex);
            chunksQueued.erase(pos);
        }
        {
            std::lock_guard lock(finishedMutex);
            chunksFinished.push(std::move(temp));
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout
            << "Generated chunk (" << pos.x << ", " << pos.y << ") in "
            << std::chrono::duration<double, std::milli>(end - start).count()
            << " ms\n";
    }
}

void World::runTasks() {
    // Load chunksFinished to finished local copy
    std::queue<std::unique_ptr<Chunk>> finished;

    {
        std::lock_guard lock(finishedMutex);
        std::swap(finished, chunksFinished);
    }

    while (!finished.empty()) {
        auto chunk = std::move(finished.front());
        finished.pop();

        chunk->init();
        chunk->uploadTriangles(); // upload

        {
            std::lock_guard lock(chunksMutex);
            chunks[chunk->chunkCoord] = std::move(chunk);
        }
    }
}

void World::runAllTasks() {
    while (!chunksToLoad.empty()) {
        glm::ivec2 pos = chunksToLoad.front();
        chunksToLoad.pop();

        chunksQueued.erase(pos);

        chunks[pos] = std::make_unique<Chunk>(pos, generation);
        generateMesh(pos);
        generateMesh(pos + glm::ivec2{1, 0});
        generateMesh(pos + glm::ivec2{-1, 0});
        generateMesh(pos + glm::ivec2{0, 1});
        generateMesh(pos + glm::ivec2{0, -1});
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

Block World::getBlock(glm::vec3 position) {
    static Block air = Block::AIR;

    int chunkX = static_cast<int>(std::floor(position.x / 16.0f));
    int chunkZ = static_cast<int>(std::floor(position.z / 16.0f));

    int localX = static_cast<int>(std::floor(position.x)) - chunkX * 16;
    int localZ = static_cast<int>(std::floor(position.z)) - chunkZ * 16;

    {
        std::lock_guard lock(chunksMutex);
        auto it = chunks.find({chunkX, chunkZ});
        if (it == chunks.end()) {
            return air;
        }

        return it->second->getBlock(
            {localX, static_cast<int>(std::floor(position.y)), localZ});
    }
}

Block World::getBlockCPU(Chunk &chunk, glm::ivec3 position) {
    auto x = position.x;
    auto y = position.y;
    auto z = position.z;

    if (y < 0 || y >= 256)
        return Block::AIR;

    if (x >= 0 && x < 16 && z >= 0 && z < 16) {
        return chunk.getBlock({x, y, z});
    }

    glm::ivec3 worldPos{chunk.chunkCoord.x * 16 + x, y,
                        chunk.chunkCoord.y * 16 + z};

    return generation.getBlock(worldPos);
}

void World::generateMesh(glm::ivec2 position) {
    auto it = chunks.find(position);

    if (it == chunks.end())
        return;

    Chunk *chunk = it->second.get();

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 256; j++) {
            for (int k = 0; k < 16; k++) {
                glm::vec3 worldPos{position.x * 16 + i, j, position.y * 16 + k};

                Block b = getBlock(worldPos);

                if (b == Block::AIR)
                    continue;

                if (getBlock(worldPos + glm::vec3{0, 0, -1}) == Block::AIR)
                    chunk->addFace(b, Face::BACK, worldPos);

                if (getBlock(worldPos + glm::vec3{0, 0, 1}) == Block::AIR)
                    chunk->addFace(b, Face::FRONT, worldPos);

                if (getBlock(worldPos + glm::vec3{0, 1, 0}) == Block::AIR)
                    chunk->addFace(b, Face::TOP, worldPos);

                if (getBlock(worldPos + glm::vec3{0, -1, 0}) == Block::AIR)
                    chunk->addFace(b, Face::BOTTOM, worldPos);

                if (getBlock(worldPos + glm::vec3{-1, 0, 0}) == Block::AIR)
                    chunk->addFace(b, Face::LEFT, worldPos);

                if (getBlock(worldPos + glm::vec3{1, 0, 0}) == Block::AIR)
                    chunk->addFace(b, Face::RIGHT, worldPos);
            }
        }
    }

    chunk->uploadTriangles();
}

void World::generateMeshCPU(std::unique_ptr<Chunk> &chunk) {
    auto position = chunk->chunkCoord;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 256; j++) {
            for (int k = 0; k < 16; k++) {
                glm::vec3 worldPos{position.x * 16 + i, j, position.y * 16 + k};

                glm::ivec3 localPos{i, j, k};

                Block b = getBlockCPU(*chunk, localPos);

                if (b == Block::AIR)
                    continue;

                if (getBlockCPU(*chunk, localPos + glm::ivec3{0, 0, -1}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::BACK, worldPos);

                if (getBlockCPU(*chunk, localPos + glm::ivec3{0, 0, 1}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::FRONT, worldPos);

                if (getBlockCPU(*chunk, localPos + glm::ivec3{0, 1, 0}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::TOP, worldPos);

                if (getBlockCPU(*chunk, localPos + glm::ivec3{0, -1, 0}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::BOTTOM, worldPos);

                if (getBlockCPU(*chunk, localPos + glm::ivec3{-1, 0, 0}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::LEFT, worldPos);

                if (getBlockCPU(*chunk, localPos + glm::ivec3{1, 0, 0}) ==
                    Block::AIR)
                    chunk->addFace(b, Face::RIGHT, worldPos);
            }
        }
    }
}
