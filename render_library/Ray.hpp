#pragma once

#include "Common.hpp"
#include "Vec3.hpp"


class Ray {
public:
    // Data
    Vec3 origin;
    Vec3 direction;
    rreal time;

public:
    explicit Ray() NOEXCEPT = default;
    explicit Ray(const Vec3 &origin_, const Vec3 &direction_, const rreal time_=0) NOEXCEPT :
        origin(origin_),
        direction(direction_),
        time(time_)
    { }

    inline Vec3 at(const rreal t) const NOEXCEPT {
        return origin + (t * direction);
    }
};
