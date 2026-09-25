#include "Triangle.h"
using namespace rendrx;

const std::vector<float> Triangle::flatten() {
    return {p1.x, p1.y, p1.z, u1.x, u1.y, p2.x, p2.y, p2.z,
            u2.x, u2.y, p3.x, p3.y, p3.z, u3.x, u3.y};
}
