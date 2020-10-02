#pragma once

#include "Interfaces/IHittable.h"
#include "AABB.h"


class RotateY : public IHittable {
private:
    // Data
    std::shared_ptr<IHittable> _obj = nullptr;
    rreal _sin_theta = 0, _cos_theta = 0;
    bool _has_box = false;
    AABB _bbox{};

public:
    explicit RotateY(const std::shared_ptr<IHittable> &obj, const rreal angle) NOEXCEPT;

    virtual std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};
