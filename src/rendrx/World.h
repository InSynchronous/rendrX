#pragma once

#include "Chunk.h"
#include "rendrx/Noise.h"
#include <condition_variable>
#include <glm/fwd.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>

namespace rendrx {
class World {
  private:
    int seed;
    std::queue<glm::ivec2> chunksToLoad;
    std::queue<glm::ivec2> chunksFinished;
    std::mutex queueMutex;
    std::mutex finishedMutex;
    std::condition_variable queueCV;
    std::thread generationThread;
    bool running = true;

    // Custom hash needed for integer vector2
    struct IVec2Hash {
        std::size_t operator()(const glm::ivec2 &v) const {
            std::size_t h1 = std::hash<int>{}(v.x);
            std::size_t h2 = std::hash<int>{}(v.y);

            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> chunks;
    std::unordered_set<glm::ivec2, IVec2Hash> chunksQueued;

    Noise noise;

    void generationLoop();

    friend class Scene;

  public:
    World(int seed) : seed(seed), noise(seed) {
        generationThread = std::thread(&World::generationLoop, this);
    }

    ~World() {
        {
            std::lock_guard lock(queueMutex);
            running = false;
        }

        queueCV.notify_one();

        if (generationThread.joinable())
            generationThread.join();
    }

    void loadChunks(glm::ivec2 position, size_t radius);
    void unloadChunks(glm::ivec2 position, size_t radius);
    void runTasks();
    void runAllTasks();
    void generateMesh(glm::ivec2 position);
    void generateMeshCPU(glm::ivec2 position);
    Block &getBlock(glm::vec3 position);
};
} // namespace rendrx
