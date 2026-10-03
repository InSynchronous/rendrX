#include "Chunk.h"
#include "Atlas.h"
#include "rendrx/Block.h"
#include "rendrx/Quad.h"
#include <algorithm>
#include <iostream>

using namespace rendrx;

Chunk::Chunk(glm::ivec2 chunkCoord, Generation &generation)
    : chunkCoord(chunkCoord), generation(generation) {

    for (auto &a : blocks)
        for (auto &b : a)
            b.fill(Block::AIR);

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {

            glm::vec2 worldCoord = (chunkCoord * 16) + glm::ivec2{x, z};
            auto height = generation.getHeight(worldCoord);
            for (int y = 0; y <= height; y++) {
                blocks[x][y][z] = generation.getBlock(y, height);
            }
        }
    }
}

void Chunk::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Block Chunk::getBlock(glm::vec3 position) {
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

void Chunk::clearMesh() { vertices.clear(); }

void Chunk::setBlock(glm::vec3 position, Block block) {
    int x = position.x;
    int y = position.y;
    int z = position.z;

    if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16) {
        return;
    }

    blocks[x][y][z] = block;
}

void Chunk::addVertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal) {
    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    vertices.push_back(uv.x);
    vertices.push_back(uv.y);

    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);
}

void Chunk::addQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
                    UV uv) {
    glm::vec2 u1{uv.min.x, uv.min.y};
    glm::vec2 u2{uv.max.x, uv.min.y};
    glm::vec2 u3{uv.max.x, uv.max.y};
    glm::vec2 u4{uv.min.x, uv.max.y};

    glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

    // Triangle 1
    addVertex(p1, u1, normal);
    addVertex(p2, u2, normal);
    addVertex(p3, u3, normal);

    // Triangle 2
    addVertex(p1, u1, normal);
    addVertex(p3, u3, normal);
    addVertex(p4, u4, normal);
}

void Chunk::addFace(Block type, Face face, glm::vec3 pos) {
    UV uv = blockToUV[static_cast<int>(type)][static_cast<int>(face)];

    switch (face) {

    case Face::FRONT:
        addQuad(pos + glm::vec3{0, 0, 1}, pos + glm::vec3{1, 0, 1},
                pos + glm::vec3{1, 1, 1}, pos + glm::vec3{0, 1, 1}, uv);
        break;

    case Face::BACK:
        addQuad(pos + glm::vec3{1, 0, 0}, pos + glm::vec3{0, 0, 0},
                pos + glm::vec3{0, 1, 0}, pos + glm::vec3{1, 1, 0}, uv);
        break;

    case Face::TOP:
        addQuad(pos + glm::vec3{0, 1, 1}, pos + glm::vec3{1, 1, 1},
                pos + glm::vec3{1, 1, 0}, pos + glm::vec3{0, 1, 0}, uv);
        break;

    case Face::BOTTOM:
        addQuad(pos + glm::vec3{0, 0, 0}, pos + glm::vec3{1, 0, 0},
                pos + glm::vec3{1, 0, 1}, pos + glm::vec3{0, 0, 1}, uv);
        break;

    case Face::RIGHT:
        addQuad(pos + glm::vec3{1, 0, 1}, pos + glm::vec3{1, 0, 0},
                pos + glm::vec3{1, 1, 0}, pos + glm::vec3{1, 1, 1}, uv);
        break;

    case Face::LEFT:
        addQuad(pos + glm::vec3{0, 0, 0}, pos + glm::vec3{0, 0, 1},
                pos + glm::vec3{0, 1, 1}, pos + glm::vec3{0, 1, 0}, uv);
        break;
    }
}

void Chunk::uploadTriangles() {
    vertex_count = vertices.size() / 8; // 5 floats per vertex

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // select our vbo

    // Upload vertecies to the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, // layout location = 0 as seen in frag shader
                          3, // 3 components per vertex (Xyz)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          8 * sizeof(float), // vertex size in vbo
                          (void *)0          // no offset (0)
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // layout location = 1 as seen in frag shader
                          2, // 2 components per vertex (uv)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          8 * sizeof(float),          // vertex size in vbo
                          (void *)(3 * sizeof(float)) // skip xyz offset (3)
    );

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, // layout location = 1 as seen in frag shader
                          3, // 2 components per vertex (normal)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          8 * sizeof(float),          // vertex size in vbo
                          (void *)(5 * sizeof(float)) // skip xyz+uv offset (5)
    );

    glEnableVertexAttribArray(2);
}

void Chunk::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
