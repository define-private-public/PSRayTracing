#pragma once

#include "Interfaces/IHittable.hpp"
#include "AABB.hpp"


class RotateY : public IHittable {
private:
    // Data
    std::shared_ptr<IHittable> _obj = nullptr;
    rreal _sin_theta = 0, _cos_theta = 0;
    bool _has_box = false;
    AABB _bbox{};

public:
    explicit RotateY(const std::shared_ptr<IHittable> &obj, const rreal angle) NOEXCEPT;

    std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;

    rreal pdf_value(
        [[maybe_unused]] RandomGenerator &rng,
        [[maybe_unused]] const Vec3 &origin,
        [[maybe_unused]] const Vec3 &v
    ) const NOEXCEPT override {
        return 0;
    }

    Vec3 random(
        [[maybe_unused]] RandomGenerator &rng,
        [[maybe_unused]] const Vec3 &origin
    ) const NOEXCEPT override {
        return Vec3(1, 0, 0);
    }
};
