#pragma once

#include "Interfaces/IHittable.h"


class Sphere : public IHittable {
public:
    // Data
    Vec3 _center = Vec3();
    rreal _radius = 0;
    std::shared_ptr<IMaterial> _mat_ptr = nullptr;

public:
    Sphere() = default;
    Sphere(const Vec3 &cen, const rreal r, const std::shared_ptr<IMaterial> &mat) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};
