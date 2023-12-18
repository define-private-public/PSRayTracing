#include "Perlin.hpp"
#include <cmath>
#include "RandomGenerator.hpp"

using namespace std;


Perlin::Perlin(RandomGenerator &rng) NOEXCEPT {
    #ifdef USE_BOOK_PERLIN
        _ranvec = new Vec3[_PerlinPointCount];
        for (int i = 0; i < _PerlinPointCount; ++i)
            _ranvec[i] = rng.get_vec3(-1, 1).unit_vector();

        _perm_x = _perlin_generate_perm(rng);
        _perm_y = _perlin_generate_perm(rng);
        _perm_z = _perlin_generate_perm(rng);
    #else  // USE_BOOK_PERLIN
        // Initilize
        for (size_t i = 0; i < _ranvec.size(); i++) {
            _ranvec[i] = rng.get_vec3(-1, 1).unit_vector();
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
Perlin::~Perlin() NOEXCEPT {
    // Cleanup memory
    delete[] _ranvec;
    delete[] _perm_x;
    delete[] _perm_x;
    delete[] _perm_y;
}

int *Perlin::_perlin_generate_perm(RandomGenerator &rng) NOEXCEPT {
    auto p = new int[_PerlinPointCount];

    for (int i = 0; i < _PerlinPointCount; i++)
        p[i] = i;

    _permute(rng, p, _PerlinPointCount);

    return p;
}
#endif // USE_BOOK_PERLIN

void Perlin::_permute(RandomGenerator &rng, int *p, int n) NOEXCEPT {
    for (int i = (n - 1); i > 0; i--) {
        int target = rng.get_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

rreal Perlin::_perlin_interp(const Vec3 c[2][2][2], const rreal u, const rreal v, const rreal w) const NOEXCEPT {
    // Hermetic cublic smoothing
    const rreal uu = u * u * (3 - (2 * u));
    const rreal vv = v * v * (3 - (2 * v));
    const rreal ww = w * w * (3 - (2 * w));      // (´・ω・｀)

    rreal accum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                const auto ri = static_cast<rreal>(i);
                const auto rj = static_cast<rreal>(j);
                const auto rk = static_cast<rreal>(k);

                const Vec3 weight_v(u - ri, v - rj, w - rk);

                accum += ((ri * uu) + ((1 - ri) * (1 - uu)))
                       * ((rj * vv) + ((1 - rj) * (1 - vv)))
                       * ((rk * ww) + ((1 - rk) * (1 - ww)))
                       * c[i][j][k].dot(weight_v);
            }
        }
    }

    return accum;
}

rreal Perlin::noise(const Vec3 &p) const NOEXCEPT {
    const auto i = static_cast<int>(floor(p.x));
    const auto j = static_cast<int>(floor(p.y));
    const auto k = static_cast<int>(floor(p.z));
    const rreal u = p.x - static_cast<rreal>(i);
    const rreal v = p.y - static_cast<rreal>(j);
    const rreal w = p.z - static_cast<rreal>(k);
    Vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                #ifdef USE_BOOK_PERLIN
                    c[di][dj][dk] =_ranvec[
                        _perm_x[(i + di) & 0xFF] ^
                        _perm_y[(j + dj) & 0xFF] ^
                        _perm_z[(k + dk) & 0xFF]
                    ];
                #else
                    // (Need to supress a bunch of compiler warnings)
                    c[di][dj][dk] =_ranvec[static_cast<size_t>(
                        _perm_x[static_cast<size_t>((i + di) & 0xFF)] ^
                        _perm_y[static_cast<size_t>((j + dj) & 0xFF)] ^
                        _perm_z[static_cast<size_t>((k + dk) & 0xFF)]
                    )];
                #endif
            }
        }
    }

    return _perlin_interp(c, u, v, w);
}

rreal Perlin::turb(const Vec3 &p, const uint8_t depth) const NOEXCEPT {
    rreal accum = 0, weight = 1;
    Vec3 temp_p(p);

    for (uint8_t i = 0; i < depth; ++i) {
        accum += weight * noise(temp_p);
        weight *= static_cast<rreal>(0.5);
        temp_p *= 2;
    }

    return std::abs(accum);
}
