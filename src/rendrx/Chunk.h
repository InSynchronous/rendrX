#pragma once

#include "Block.h"
#include <array>

namespace rendrx {
class Chunk {
  private:
    std::array<std::array<std::array<Block, 16>, 16>, 16> blocks;
    glm::vec2 chunkCoord; // 1 = 16 blocks

  public:
    Chunk(glm::ivec2 chunkCoord);
    Block &getBlock(glm::vec3 position);
};
} // namespace rendrx
