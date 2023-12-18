#pragma once

#include "Interfaces/IHittable.hpp"


class Sphere : public IHittable {
private:
    // Data
    Vec3 _center = Vec3();
    rreal _radius = 0;
    std::shared_ptr<IMaterial> _mat_ptr = nullptr;

public:
    explicit Sphere() NOEXCEPT = default;
    explicit Sphere(const Vec3 &cen, const rreal r, const std::shared_ptr<IMaterial> &mat) NOEXCEPT;

    std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
    rreal pdf_value(RandomGenerator &rng, const Vec3 &origin, const Vec3 &v) const NOEXCEPT override;
    Vec3 random(RandomGenerator &rng, const Vec3 &origin) const NOEXCEPT override;
};
