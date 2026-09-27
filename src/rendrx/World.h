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

  public:
    World(int seed) : seed(seed) {};

    // People should be able to read chunks and write to them, its fine
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> chunks;

    void loadChunks(glm::ivec2 position, size_t radius);
    Block &getBlock(glm::vec3 position);
};
} // namespace rendrx
