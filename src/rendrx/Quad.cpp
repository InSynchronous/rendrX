#include "Quad.h"

namespace rendrx {

const std::vector<float> Quad::flatten() {
    return {p1.x, p1.y, p1.z, u1.x, u1.y, p2.x, p2.y, p2.z,
            u2.x, u2.y, p3.x, p3.y, p3.z, u3.x, u3.y,

            p1.x, p1.y, p1.z, u1.x, u1.y, p3.x, p3.y, p3.z,
            u3.x, u3.y, p4.x, p4.y, p4.z, u4.x, u4.y};
}

} // namespace rendrx
