#include "Chunk.h"
#include "Atlas.h"
using namespace rendrx;

Chunk::Chunk(glm::ivec2 chunkCoord) : chunkCoord(chunkCoord) {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            blocks[x][0][z] = Block(
                glm::vec3{x + chunkCoord.x * 16, 0, z + chunkCoord.y * 16},
                getUV(2, 15));
        }
    }

    // setup buffers
    glGenVertexArrays(1, &VAO); // 1 triangle/unique VAO
    glGenBuffers(1, &VBO);      // 1 buffer? still unsure
}

Block &Chunk::getBlock(glm::vec3 position) {
    static Block airBlock;

    // refactor this slop bro
    int x = position.x;
    int y = position.y;
    int z = position.z;

    if (x < 0 || x >= 16 || y < 0 || y >= 16 || z < 0 || z >= 16) {
        return airBlock;
    }

    return blocks[x][y][z];
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
    vertex_count = vertices.size();

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
