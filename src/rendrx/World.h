#pragma once

#include "Chunk.h"
#include <glm/fwd.hpp>
#include <memory>
#include <unordered_map>

namespace rendrx {
class World {
  private:
    int seed;
    // Custom hash needed for integer vector2
    struct IVec2Hash {
        std::size_t operator()(const glm::ivec2 &v) const {
            std::size_t h1 = std::hash<int>{}(v.x);
            std::size_t h2 = std::hash<int>{}(v.y);

            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> chunks;
    friend class Scene;

  public:
    World(int seed) : seed(seed) {};

    void loadChunks(glm::ivec2 position, size_t radius);
    void unloadChunks(glm::ivec2 position, size_t radius);
    void generateMeshes();
    Block &getBlock(glm::vec3 position);
};
} // namespace rendrx
