#pragma once

#include "HitRecord.h"
class AABB;
class RandomGenerator;


class IHittable {
public:
    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT = 0;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT = 0;
};
