#pragma once

#include "Block.h"
#include "Object.h"
#include "rendrx/Noise.h"
#include <array>
#include <glad/gl.h>
#include <memory>

namespace rendrx {
class Chunk {
  private:
    std::array<std::array<std::array<Block, 16>, 256>, 16> blocks;
    std::vector<std::unique_ptr<Object>> objects;
    glm::vec2 chunkCoord; // 1 = 16 blocks

    Noise &noise;

    unsigned int vertex_count, VAO, VBO;

  public:
    Chunk(glm::ivec2 chunkCoord, Noise &noise);
    ~Chunk() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    Block &getBlock(glm::vec3 position);
    void addFace(Block type, Face face, glm::vec3 pos);

    void uploadTriangles();
    void addObject(std::unique_ptr<Object> o);
    void draw();
    void init();
};
} // namespace rendrx
