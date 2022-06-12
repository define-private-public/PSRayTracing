#pragma once

#include "Interfaces/IHittable.h"
#include <vector>
#include "AABB.h"
class HittableList;


class BVHNode : public IHittable {
private:
    // Data
    std::shared_ptr<IHittable> _left = nullptr;
    std::shared_ptr<IHittable> _right = nullptr;
    AABB _box;

public:
    explicit BVHNode() = default;
    explicit BVHNode(RandomGenerator &rng, const HittableList &list, const rreal time0, const rreal time1);
    explicit BVHNode(
        RandomGenerator &rng,
        std::vector<std::shared_ptr<IHittable>> objects,
        const size_t start,
        const size_t end,
        const rreal time0,
        const rreal time1
    );

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

// Box comparisons
bool box_compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b, const Vec3::Axis &axis);

inline bool box_x_compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
    return box_compare(a, b, Vec3::Axis::X);
}

inline bool box_y_compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
    return box_compare(a, b, Vec3::Axis::Y);
}

inline bool box_z_compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
    return box_compare(a, b, Vec3::Axis::Z);
}
