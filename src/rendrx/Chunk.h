#pragma once

#include "Block.h"
#include <array>

namespace rendrx {
class Chunk {
  private:
    std::array<std::array<std::array<Block, 16>, 16>, 16> blocks;

  public:
    Chunk();
    Block &getBlock(int x, int y, int z);
};
} // namespace rendrx
