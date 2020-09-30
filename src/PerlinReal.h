#pragma once

#include "Vec3.h"
class RandomGenerator;


// How many random points to create
constexpr int _PerlinRealPointCount = 256;


// TODO [refactoring]
// - There are also some other third-party noise implementations, they might be more worth
//   while to use

// A simple perlin noise generator, using our `rreal` type
class PerlinReal {
public:
    enum class InterpolationType {
        None,
        TriLinear,
        TriLinearHermiteCubic
    };

private:
    // Data
    InterpolationType _interp_method;

    #ifdef USE_BOOK_PERLIN
        // The book uses dynamic memory for these objects
        rreal *_ranreal;
        int *_perm_x;
        int *_perm_y;
        int *_perm_z;
    #else  // USE_BOOK_PERLIN
        std::array<rreal, _PerlinRealPointCount> _ranreal;
        std::array<int, _PerlinRealPointCount> _perm_x;
        std::array<int, _PerlinRealPointCount> _perm_y;
        std::array<int, _PerlinRealPointCount> _perm_z;
    #endif // USE_BOOK_PERLIN

public:
    PerlinReal(RandomGenerator &rng, const InterpolationType interpMethod) NOEXCEPT;

    #ifdef USE_BOOK_PERLIN
        ~PerlinReal() NOEXCEPT;
    #endif

private:
    #ifdef USE_BOOK_PERLIN
        static int *_perlin_generate_perm(RandomGenerator &rng) NOEXCEPT;    // Returns dynamically allocated memory
    #endif

    static void _permute(RandomGenerator &rng, int *p, int n) NOEXCEPT;

    rreal _noise_no_interp(const Vec3 &p) const NOEXCEPT;
    rreal _noise_trilinear_interp(const Vec3 &p, const bool hermitian_smoothing) const NOEXCEPT;

    inline rreal _noise_trilinear_interp(const Vec3 &p) const NOEXCEPT {
        return _noise_trilinear_interp(p, false);
    }

    inline rreal _noise_trilinear_interp_smooth(const Vec3 &p) const NOEXCEPT {
        return _noise_trilinear_interp(p, true);
    }

public:
    inline rreal noise(const Vec3 &p) const NOEXCEPT {
        // Figure out which interpolation method should be used
        if (_interp_method == InterpolationType::TriLinear)
            return _noise_trilinear_interp(p);
        else if (_interp_method == InterpolationType::TriLinearHermiteCubic)
            return _noise_trilinear_interp_smooth(p);

        return _noise_no_interp(p);     // Default do no interpolation
    }
};
