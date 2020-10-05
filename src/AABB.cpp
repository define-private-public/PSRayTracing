#include "AABB.h"
#include <cmath>
#include <algorithm>
#include <utility>
#include "Ray.h"


AABB::AABB(const Vec3 &a, const Vec3 &b) NOEXCEPT  :
    min(a),
    max(b)
{ }

/*
AABB::AABB(const AABB &other) NOEXCEPT  :
    AABB(other.min, other.max)
{ }
*/

AABB AABB::surrounding(const AABB &box0, const AABB &box1) NOEXCEPT  {
    const Vec3 small(
        std::min(box0.min.x, box1.min.x),
        std::min(box0.min.y, box1.min.y),
        std::min(box0.min.z, box1.min.z)
    );

    const Vec3 big(
        std::max(box0.max.x, box1.max.x),
        std::max(box0.max.y, box1.max.y),
        std::max(box0.max.z, box1.max.z)
    );

    return AABB(small, big);
}

/*
AABB &AABB::operator=(const AABB &other) NOEXCEPT  {
    // Samsies?
    if (this == &other)
        return *this;

    // Assing
    min = other.min;
    max = other.max;
    return *this;
}
*/

bool AABB::hit(const Ray &r, rreal tmin, rreal tmax) const NOEXCEPT {
#ifdef USE_BOOK_AABB_HIT
    // Becuase of how I built my Vec3 object, I need to write out
    // each axis...
    //*== Andrew Kensler (Pixar) method ==*/
    rreal inv_d, t0, t1;

    // X-axis
    inv_d = static_cast<rreal>(1) / r.direction.x;
    t0 = (min.x - r.origin.x) * inv_d;
    t1 = (max.x - r.origin.x) * inv_d;
    if (inv_d < 0)
        std::swap(t0, t1);
    tmin = (t0 > tmin) ? t0 : tmin;
    tmax = (t1 < tmax) ? t1 : tmax;
    if (tmax <= tmin)
        return false;

    // Y-axis
    inv_d = static_cast<rreal>(1) / r.direction.y;
    t0 = (min.y - r.origin.y) * inv_d;
    t1 = (max.y - r.origin.y) * inv_d;
    if (inv_d < 0)
        std::swap(t0, t1);
    tmin = (t0 > tmin) ? t0 : tmin;
    tmax = (t1 < tmax) ? t1 : tmax;
    if (tmax <= tmin)
        return false;

    // Z-axis
    inv_d = static_cast<rreal>(1) / r.direction.z;
    t0 = (min.z - r.origin.z) * inv_d;
    t1 = (max.z - r.origin.z) * inv_d;
    if (inv_d < 0)
        std::swap(t0, t1);
    tmin = (t0 > tmin) ? t0 : tmin;
    tmax = (t1 < tmax) ? t1 : tmax;
    if (tmax <= tmin)
        return false;

    return true;
#else
    // This method is a bit faster because of modern CPUs (vector instructions)
    // and newer compilers (doining auto-vectorization)

    // Adapted from: https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
    const rreal t0[3] = {
        (min.x - r.origin.x) / r.direction.x,
        (min.y - r.origin.y) / r.direction.y,
        (min.z - r.origin.z) / r.direction.z,
    };
    const rreal t1[3] = {
        (max.x - r.origin.x) / r.direction.x,
        (max.y - r.origin.y) / r.direction.y,
        (max.z - r.origin.z) / r.direction.z,
    };

    const rreal t_smaller[3] = {
        std::min(t0[0], t1[0]),
        std::min(t0[1], t1[1]),
        std::min(t0[2], t1[2])
    };
    const rreal t_larger[3] = {
        std::max(t0[0], t1[0]),
        std::max(t0[1], t1[1]),
        std::max(t0[2], t1[2])
    };

    tmin = std::max({tmin, t_smaller[0], t_smaller[1], t_smaller[2]});
    tmax = std::min({tmax, t_larger[0], t_larger[1], t_larger[2]});

    return (tmin < tmax);
#endif
}
