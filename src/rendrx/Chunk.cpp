#include "Chunk.h"
#include "Atlas.h"
#include "rendrx/Block.h"
#include "rendrx/Quad.h"
#include <algorithm>
#include <iostream>

using namespace rendrx;

Chunk::Chunk(glm::ivec2 chunkCoord, Noise &noise)
    : chunkCoord(chunkCoord), noise(noise) {

    for (auto &a : blocks)
        for (auto &b : a)
            b.fill(Block::AIR);

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {

            glm::ivec2 worldCoord = (chunkCoord * 16) + glm::ivec2{x, z};

            float n = noise.fbm(glm::vec2(worldCoord) * 0.015f, // terrain scale
                                5,                              // octaves
                                1.0f,                           // amplitude
                                1.0f,                           // frequency
                                2.0f,                           // lacunarity
                                0.5f                            // persistence
            );

            int height = 80 + static_cast<int>(n * 35.0f);

            height = std::clamp(height, 1, 255);

            for (int y = 0; y <= height; y++) {
                if (y > 80)
                    blocks[x][y][z] = Block::DIRT;
                else
                    blocks[x][y][z] = Block::STONE;

                if (y == height)
                    blocks[x][y][z] = Block::GRASS;
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Block &Chunk::getBlock(glm::vec3 position) {
    static Block airBlock = Block::AIR;

    // refactor this slop bro
    int x = position.x;
    int y = position.y;
    int z = position.z;

    if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16) {
        return airBlock;
    }

    return blocks[x][y][z];
}

void Chunk::addFace(Block type, Face face, glm::vec3 pos) {
    float s = 0.5f;

    UV uv = blockToUV[static_cast<int>(type)][static_cast<int>(face)];

    switch (face) {
    case Face::FRONT:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, +s}, pos + glm::vec3{+s, -s, +s},
            pos + glm::vec3{+s, +s, +s}, pos + glm::vec3{-s, +s, +s}, uv));
        break;

    case Face::BACK:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{+s, -s, -s}, pos + glm::vec3{-s, -s, -s},
            pos + glm::vec3{-s, +s, -s}, pos + glm::vec3{+s, +s, -s}, uv));
        break;

    case Face::TOP:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{+s, +s, +s},
            pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{-s, +s, -s}, uv));
        break;

    case Face::BOTTOM:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{+s, -s, -s},
            pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{-s, -s, +s}, uv));
        break;

    case Face::RIGHT:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{+s, -s, +s}, pos + glm::vec3{+s, -s, -s},
            pos + glm::vec3{+s, +s, -s}, pos + glm::vec3{+s, +s, +s}, uv));
        break;

    case Face::LEFT:
        addObject(std::make_unique<Quad>(
            pos + glm::vec3{-s, -s, -s}, pos + glm::vec3{-s, -s, +s},
            pos + glm::vec3{-s, +s, +s}, pos + glm::vec3{-s, +s, -s}, uv));
        break;
    }
}

void Chunk::addObject(std::unique_ptr<Object> o) {
    objects.push_back(std::move(o));
}

void Chunk::uploadTriangles() {
    // Flatten the array
    std::vector<float> vertices;
    for (const std::unique_ptr<Object> &obj : objects) {
        auto v = obj->flatten();
        vertices.insert(vertices.end(), v.begin(), v.end());
    }
    objects.clear();
    vertex_count = vertices.size() / 5; // 5 floats per vertex

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // select our vbo

    // Upload vertecies to the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, // layout location = 0 as seen in frag shader
                          3, // 3 components per vertex (Xyz)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float), // vertex size in vbo
                          (void *)0          // no offset (0)
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // layout location = 1 as seen in frag shader
                          2, // 2 components per vertex (uv)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float),          // vertex size in vbo
                          (void *)(3 * sizeof(float)) // skip xyz offset (3)
    );

    glEnableVertexAttribArray(1);
}

void Chunk::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
