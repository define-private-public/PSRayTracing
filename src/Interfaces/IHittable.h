#pragma once

#include "HitRecord.h"
#include "IDeepCopyable.h"
class AABB;
class RandomGenerator;


class IHittable : public IDeepCopyable<IHittable> {
public:
    virtual ~IHittable() NOEXCEPT = default;

    virtual std::shared_ptr<IHittable> deep_copy() const NOEXCEPT = 0;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT = 0;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT = 0;
};
