#include "PerlinReal.h"
#include <cmath>
#include "RandomGenerator.h"

using namespace std;


inline rreal _trilinear_interp(const rreal c[2][2][2], const rreal u, const rreal v, const rreal w) NOEXCEPT;


PerlinReal::PerlinReal(RandomGenerator &rng, const InterpolationType interpMethod) NOEXCEPT :
    _interp_method(interpMethod)
{
    #ifdef USE_BOOK_PERLIN
        _ranreal = new rreal[_PerlinRealPointCount];
        for (int i = 0; i < _PerlinRealPointCount; ++i)
            _ranreal[i] = rng.get_real();

        _perm_x = _perlin_generate_perm(rng);
        _perm_y = _perlin_generate_perm(rng);
        _perm_z = _perlin_generate_perm(rng);
    #else  // USE_BOOK_PERLIN
        // Initilize
        for (size_t i = 0; i < _ranreal.size(); i++) {
            _ranreal[i] = rng.get_real();
            _perm_x[i] = static_cast<int>(i);
            _perm_y[i] = static_cast<int>(i);
            _perm_z[i] = static_cast<int>(i);
        }

        _permute(rng, _perm_x.data(), static_cast<int>(_perm_x.size()));
        _permute(rng, _perm_y.data(), static_cast<int>(_perm_y.size()));
        _permute(rng, _perm_z.data(), static_cast<int>(_perm_z.size()));
    #endif // USE_BOOK_PERLIN
}

#ifdef USE_BOOK_PERLIN
PerlinReal::~PerlinReal() NOEXCEPT {
    // Cleanup memory
    delete[] _ranreal;
    delete[] _perm_x;
    delete[] _perm_x;
    delete[] _perm_y;
}

int *PerlinReal::_perlin_generate_perm(RandomGenerator &rng) NOEXCEPT {
    auto p = new int[_PerlinRealPointCount];

    for (int i = 0; i < _PerlinRealPointCount; i++)
        p[i] = i;

    _permute(rng, p, _PerlinRealPointCount);

    return p;
}
#endif // USE_BOOK_PERLIN

void PerlinReal::_permute(RandomGenerator &rng, int *p, int n) NOEXCEPT {
    for (int i = (n - 1); i > 0; i--) {
        int target = rng.get_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

rreal PerlinReal::_noise_trilinear_interp(const Vec3 &p, const bool hermitian_smoothing) const NOEXCEPT {
    const auto i = static_cast<int>(floor(p.x));
    const auto j = static_cast<int>(floor(p.y));
    const auto k = static_cast<int>(floor(p.z));
    rreal u = p.x - static_cast<rreal>(i);
    rreal v = p.y - static_cast<rreal>(j);
    rreal w = p.z - static_cast<rreal>(k);

    if (hermitian_smoothing) {
        u = u * u * (3 - (2 * u));
        v = v * v * (3 - (2 * v));
        w = w * w * (3 - (2 * w));
    }

    rreal c[2][2][2];

    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] =_ranreal[static_cast<size_t>(
                    _perm_x[static_cast<size_t>(i + di) & 0xFF] ^
                    _perm_y[static_cast<size_t>(j + dj) & 0xFF] ^
                    _perm_z[static_cast<size_t>(k + dk) & 0xFF]
                )];
            }
        }
    }

    return _trilinear_interp(c, u, v, w);
}

rreal PerlinReal::_noise_no_interp(const Vec3 &p) const NOEXCEPT {
    const auto i = static_cast<size_t>(4 * p.x) & 0xFF;
    const auto j = static_cast<size_t>(4 * p.y) & 0xFF;
    const auto k = static_cast<size_t>(4 * p.z) & 0xFF;

    return _ranreal[static_cast<size_t>(_perm_x[i] ^ _perm_y[j] ^ _perm_z[k])];
}

inline rreal _trilinear_interp(const rreal c[2][2][2], const rreal u, const rreal v, const rreal w) NOEXCEPT {
    rreal accum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                accum += ((static_cast<rreal>(i) * u) + ((1 - static_cast<rreal>(i)) * (1 - u)))
                       * ((static_cast<rreal>(j) * v) + ((1 - static_cast<rreal>(j)) * (1 - v)))
                       * ((static_cast<rreal>(k) * w) + ((1 - static_cast<rreal>(k)) * (1 - w)))
                       * c[i][j][k];
            }
        }
    }

    return accum;
}
