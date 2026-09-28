#include "Noise.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

using namespace rendrx;

Noise::Noise(size_t seed) {
    std::array<int, 256> p;

    std::iota(p.begin(), p.end(), 0);

    std::mt19937 rng(seed);
    std::shuffle(p.begin(), p.end(), rng);

    for (int i = 0; i < 512; i++)
        perm[i] = p[i & 255];
}

float Noise::fade(float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float Noise::lerp(float a, float b, float t) { return a + t * (b - a); }

float Noise::gradient(int hash, glm::vec2 vec) {
    float x = vec.x;
    float y = vec.y;
    switch (hash & 3) {
    case 0:
        return x + y;
    case 1:
        return -x + y;
    case 2:
        return x - y;
    default:
        return -x - y;
    }
}

float Noise::perlin(glm::vec2 vec) {
    float x = vec.x;
    float y = vec.y;
    int x0 = static_cast<int>(std::floor(x)) & 255;
    int y0 = static_cast<int>(std::floor(y)) & 255;

    int x1 = (x0 + 1) & 255;
    int y1 = (y0 + 1) & 255;

    float dx = x - std::floor(x);
    float dy = y - std::floor(y);

    float u = fade(dx);
    float v = fade(dy);

    int aa = perm[perm[x0] + y0];
    int ab = perm[perm[x0] + y1];
    int ba = perm[perm[x1] + y0];
    int bb = perm[perm[x1] + y1];

    float n00 = gradient(aa, {dx, dy});
    float n10 = gradient(ba, {dx - 1, dy});
    float n01 = gradient(ab, {dx, dy - 1});
    float n11 = gradient(bb, {dx - 1, dy - 1});

    float nx0 = lerp(n00, n10, u);
    float nx1 = lerp(n01, n11, u);

    return lerp(nx0, nx1, v);
}

float Noise::fbm(glm::vec2 position, int octaves, float amplitude,
                 float frequency, float lacunarity, float persistence) {
    float value = 0.0f;

    for (int i = 0; i < octaves; i++) {
        value += perlin(position * frequency) * amplitude;

        frequency *= lacunarity;
        amplitude *= persistence;
    }

    return value;
}
