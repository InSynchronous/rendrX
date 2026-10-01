#pragma once

#include "Block.h"
#include "Object.h"
#include "rendrx/Generation.h"
#include "rendrx/Noise.h"
#include <array>
#include <glad/gl.h>
#include <memory>

namespace rendrx {
class Chunk {
  private:
    std::array<std::array<std::array<Block, 16>, 256>, 16> blocks;
    std::vector<float> vertices;
    void addVertex(glm::vec3 pos, glm::vec2 uv);
    void addQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, UV uv);

    Generation &generation;

    unsigned int vertex_count, VAO, VBO;

  public:
    glm::vec2 chunkCoord; // 1 = 16 blocks

    Chunk(glm::ivec2 chunkCoord, Generation &generation);
    ~Chunk() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    Block getBlock(glm::vec3 position);
    void addFace(Block type, Face face, glm::vec3 pos);

    void uploadTriangles();
    void draw();
    void init();
};
} // namespace rendrx
