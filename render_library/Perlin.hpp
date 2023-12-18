#pragma once

#include "Vec3.hpp"
#include <cstdint>
#include <array>
class RandomGenerator;


// How many random points to create
constexpr int _PerlinPointCount = 256;


// A simple perlin noise generator, useing our `Vec3` type
class Perlin {
private:
    // Data
    #ifdef USE_BOOK_PERLIN
        // The book uses dynamic memory for these objects
        Vec3 *_ranvec;
        int *_perm_x;
        int *_perm_y;
        int *_perm_z;
    #else  // USE_BOOK_PERLIN
        // Out implementation uses std::array (and stack memory)
        std::array<Vec3, _PerlinPointCount> _ranvec;
        std::array<int, _PerlinPointCount> _perm_x;
        std::array<int, _PerlinPointCount> _perm_y;
        std::array<int, _PerlinPointCount> _perm_z;
    #endif // USE_BOOK_PERLIN


public:
    explicit Perlin(RandomGenerator &rng) NOEXCEPT;

    #ifdef USE_BOOK_PERLIN
        ~Perlin() NOEXCEPT;
    #endif

private:
    #ifdef USE_BOOK_PERLIN
        static int *_perlin_generate_perm(RandomGenerator &rng) NOEXCEPT;    // Returns dynamically allocated memory
    #endif

    static void _permute(RandomGenerator &rng, int *p, int n) NOEXCEPT;

    rreal _perlin_interp(const Vec3 c[2][2][2], const rreal u, const rreal v, const rreal w) const NOEXCEPT;

public:
    rreal noise(const Vec3 &p) const NOEXCEPT;
    rreal turb(const Vec3 &p, const uint8_t depth=7) const NOEXCEPT;
};
