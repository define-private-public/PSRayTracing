#pragma once

#include "Interfaces/IHittable.h"


class MovingSphere : public IHittable {
private:
    // Data
    Vec3 _center0 = Vec3(), _center1 = Vec3();
    rreal _time0 = 0, _time1 = 0;
    rreal _radius = 0;
    std::shared_ptr<IMaterial> _mat_ptr = nullptr;

public:
    explicit MovingSphere() NOEXCEPT = default;
    explicit MovingSphere(
        const Vec3 &center0, const Vec3 &center1,
        const rreal time0, const rreal time1,
        const rreal r,
        const std::shared_ptr<IMaterial> &mat_ptr
    ) NOEXCEPT;

    std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;

    Vec3 center(const rreal time) const NOEXCEPT;
};
