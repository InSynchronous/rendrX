#pragma once

#include "Block.h"
#include <array>
#include <glad/gl.h>

namespace rendrx {
class Chunk {
  private:
    std::array<std::array<std::array<Block, 16>, 16>, 16> blocks;
    std::vector<std::unique_ptr<Object>> objects;
    glm::vec2 chunkCoord; // 1 = 16 blocks

    unsigned int vertex_count, VAO, VBO;

  public:
    Chunk(glm::ivec2 chunkCoord);
    ~Chunk() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    Block &getBlock(glm::vec3 position);
    void uploadTriangles();
    void addObject(std::unique_ptr<Object> o);
    void draw();
};
} // namespace rendrx
