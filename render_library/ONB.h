#pragma once

#include "Vec3.h"

// An Orthonormal Basis utility class
class ONB {
public:
    // Data
    Vec3 axis[3];

    inline Vec3 operator[](const int i) const NOEXCEPT { return axis[i]; }

    inline Vec3 u() const NOEXCEPT { return axis[0]; }
    inline Vec3 v() const NOEXCEPT { return axis[1]; }
    inline Vec3 w() const NOEXCEPT { return axis[2]; }

    inline Vec3 local(const rreal a, const rreal b, const rreal c) const NOEXCEPT {
        return (a * u()) +
               (b * v()) +
               (c * w());
    }

    inline Vec3 local(const Vec3 &a) const NOEXCEPT {
        return (a.x * u()) +
               (a.y * v()) +
               (a.z * w());
    }

    void build_from_w(const Vec3 &n) NOEXCEPT;
};