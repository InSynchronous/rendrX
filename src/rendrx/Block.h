#pragma once

#include "Atlas.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace rendrx {
enum class Block { AIR, GRASS, DIRT, STONE, SAND };
enum class Face { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

// 5 Blocks, 6 Faces, UV coord set
constexpr std::array<std::array<UV, 6>, 5> blockToUV{
    {{{
         getUV({3, 15}), // front
         getUV({3, 15}), // back
         getUV({3, 15}), // left
         getUV({3, 15}), // right
         getUV({3, 15}), // top
         getUV({3, 15})  // bottom
     }},
     {{
         getUV({3, 15}), // front
         getUV({3, 15}), // back
         getUV({3, 15}), // left
         getUV({3, 15}), // right
         getUV({2, 6}),  // top
         getUV({2, 15})  // bottom
     }},
     {{
         getUV({2, 15}), // front
         getUV({2, 15}), // back
         getUV({2, 15}), // left
         getUV({2, 15}), // right
         getUV({2, 15}), // top
         getUV({2, 15})  // bottom
     }},
     {{
         getUV({1, 15}), // front
         getUV({1, 15}), // back
         getUV({1, 15}), // left
         getUV({1, 15}), // right
         getUV({1, 15}), // top
         getUV({1, 15})  // bottom
     }},
     {{
         getUV({0, 4}), // front
         getUV({0, 4}), // back
         getUV({0, 4}), // left
         getUV({0, 4}), // right
         getUV({0, 4}), // top
         getUV({0, 4})  // bottom
     }}}};
} // namespace rendrx
