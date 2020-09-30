#pragma once

#include "Common.h"
#include "Vec3.h"
#include "Ray.h"

struct HitRecord;
class RandomGenerator;


class IMaterial {
public:
    virtual bool scatter(
        RandomGenerator &rng,   // In
        const Ray &r_in,        // In
        const HitRecord &rec,   // In
        Vec3 &attenuation,      // Out
        Ray &scattered          // Out
    ) const NOEXCEPT = 0;

    virtual Vec3 emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT = 0;
};
