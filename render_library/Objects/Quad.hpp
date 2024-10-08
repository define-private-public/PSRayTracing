#pragma once

#include "Interfaces/IHittable.hpp"
#include "AABB.hpp"


class Quad FINAL : public IHittable
{
private:
    Vec3 _Q = Vec3();       // Acts as a point of origin for the quad
    Vec3 _u = Vec3();
    Vec3 _v = Vec3();
    Vec3 _w = Vec3();
    Vec3 _normal = Vec3();
    rreal _area = 0;
    rreal _D = 0;
    AABB _bounding_box = AABB();
    std::shared_ptr<IMaterial> _mat_ptr = nullptr;

public:
    explicit Quad(const Vec3 &origin, const Vec3 &u, const Vec3 &v, const std::shared_ptr<IMaterial> &material) NOEXCEPT;

    std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
    rreal pdf_value(RandomGenerator &rng, const Vec3 &origin, const Vec3 &v) const NOEXCEPT override;
    Vec3 random(RandomGenerator &rng, const Vec3 &origin) const NOEXCEPT override;

private:
    void _set_bounding_box();
    bool _is_interior(const rreal a, const rreal b, HitRecord &rec) const;  // New method from v4 series
};
