#pragma once

#include "Interfaces/IHittable.hpp"


class FlipFace FINAL : public IHittable {
private:
    // Data
    std::shared_ptr<IHittable> _obj;

public:
    explicit FlipFace(const std::shared_ptr<IHittable> &obj) NOEXCEPT :
        _obj(obj)
    { }

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
