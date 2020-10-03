#pragma once

#include "Common.h"
#include "Util.h"
#include <iosfwd>
#include <string>

// foward decls
class Vec3;
inline Vec3 operator+(const Vec3 &a, const Vec3 &b) NOEXCEPT;
inline Vec3 operator-(const Vec3 &a, const Vec3 &b) NOEXCEPT;
inline Vec3 operator*(const Vec3 &a, const Vec3 &b) NOEXCEPT;
inline Vec3 operator*(const rreal s, const Vec3 &v) NOEXCEPT;
inline Vec3 operator*(const Vec3 &v, const rreal s) NOEXCEPT;
inline Vec3 operator/(const Vec3 &v, const rreal s) NOEXCEPT;


class Vec3 {
public:
    // Data
    rreal x = 0;
    rreal y = 0;
    rreal z = 0;
//    rreal _unused;        // From a byte-alignment test, not sure if this actually helped or not

    enum class Axis {
        X = 0,
        Y,
        Z
    };

public:
    explicit Vec3() NOEXCEPT = default;

    // Create a vector w/ the same value for each component
    explicit Vec3(const rreal value) NOEXCEPT :
        Vec3(value, value, value)
    { }

    explicit Vec3(const rreal x_, const rreal y_, const rreal z_) NOEXCEPT :
        x(x_),
        y(y_),
        z(z_)
    { }
/*
    explicit Vec3(const Vec3 &other) NOEXCEPT :
        x(other.x),
        y(other.y),
        z(other.z)
    { }

    inline Vec3 &operator=(const Vec3 &other) NOEXCEPT {
        // Samsies check?
        if (this == &other)
            return *this;

        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }
*/
    // Negation
    inline Vec3 operator-() const NOEXCEPT {
        return Vec3(-x, -y, -z);
    }

    inline Vec3 &operator+=(const Vec3 &other) NOEXCEPT {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    inline Vec3 &operator-=(const Vec3 &other) NOEXCEPT {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    inline Vec3 &operator*=(const rreal t) NOEXCEPT {
        x *= t;
        y *= t;
        z *= t;

        return *this;
    }

    inline Vec3 &operator/=(const rreal t) NOEXCEPT {
        return (*this *= (1 / t));
    }

    inline rreal length() const NOEXCEPT {
        return util::sqrt(length_squared());
    }

    // Is the dot product with itself
    inline rreal length_squared() const NOEXCEPT {
        return (x * x) + (y * y) + (z * z);
    }

    inline Vec3 unit_vector() const NOEXCEPT {
        return *this / length();
    }

    inline rreal dot(const Vec3 &other) const NOEXCEPT {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    inline Vec3 cross(const Vec3 &other) const NOEXCEPT {
        return Vec3(
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)
        );
    }

    inline Vec3 reflect(const Vec3 &n) const NOEXCEPT {
        const Vec3 v = *this;
        return v - (2 * v.dot(n) * n);
    }

    Vec3 refract(const Vec3 &n, const rreal etai_over_etat) const NOEXCEPT {
        const Vec3 uv = *this;
        const rreal cos_theta = (-uv).dot(n);
        const Vec3 r_out_perp = etai_over_etat * (uv + (cos_theta * n));
        const Vec3 r_out_parallel = -util::sqrt(std::fabs(1 - r_out_perp.length_squared())) * n;

        return r_out_perp + r_out_parallel;
    }

    // Produces the vector but with each component their inverse
    inline Vec3 one_over() const NOEXCEPT {
        return Vec3(
            1 / x,
            1 / y,
            1 / z
        );
    }

    std::string str() const {
        return "Vec3<" + std::to_string(x) +
                  ", " + std::to_string(y) +
                  ", " + std::to_string(z) + ">";
    }
};

// More arithmetic
inline Vec3 operator+(const Vec3 &a, const Vec3 &b) NOEXCEPT {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3 &a, const Vec3 &b) NOEXCEPT {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3 &a, const Vec3 &b) NOEXCEPT {
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator*(const rreal s, const Vec3 &v) NOEXCEPT {
    return Vec3(s * v.x, s * v.y, s * v.z);
}

inline Vec3 operator*(const Vec3 &v, const rreal s) NOEXCEPT {
    return s * v;
}

inline Vec3 operator/(const Vec3 &v, const rreal s) NOEXCEPT {
    return (static_cast<rreal>(1) / s) * v;
}

// Other
std::ostream &operator<<(std::ostream &out, const Vec3 &v);
