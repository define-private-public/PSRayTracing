#pragma once

#include "Interfaces/IHittable.h"
#include "Objects/HittableList.h"
#include "AABB.h"
class BVHNode;
class RandomGenerator;


class Box : public IHittable {
private:
    // Data
    Vec3 _box_min = Vec3(0);
    Vec3 _box_max = Vec3(0);
    HittableList _sides{};

#ifndef USE_BOOK_BOX_HEIARCHY
    // It's slightly more optimized to use a BVH node in the `box` instead of a hittable list
    std::shared_ptr<BVHNode> _heiarchy;
#endif


public:
    explicit Box(RandomGenerator &rng, const Vec3 &point_min, const Vec3 &point_max, const std::shared_ptr<IMaterial> &mat) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};
