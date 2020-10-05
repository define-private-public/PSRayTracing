#pragma once

#include "Common.h"
#include "Vec3.h"

class Ray;


// An axis-aligned bounding box
class AABB {
public:
    // Data
    Vec3 min;
    Vec3 max;

public:
    explicit AABB() NOEXCEPT = default;
    explicit AABB(const Vec3 &a, const Vec3 &b) NOEXCEPT ;
//    explicit AABB(const AABB &other) NOEXCEPT ;
    static AABB surrounding(const AABB &box0, const AABB &box1) NOEXCEPT ;

//    AABB &operator=(const AABB &other) NOEXCEPT ;

    bool hit(const Ray &r, rreal tmin, rreal tmax) const NOEXCEPT ;

    std::string str() const {
        return "AABB(min=" + min.str() +
                  ", max=" + max.str() + ")";
    }
};
