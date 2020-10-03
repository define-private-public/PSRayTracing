#pragma once

#include <vector>
#include <memory>
#include "Common.h"
#include "Interfaces/IHittable.h"


class HittableList : public IHittable {
public:
    // Data
    std::vector<std::shared_ptr<IHittable>> _objects;

public:
    explicit HittableList() NOEXCEPT = default;
    explicit HittableList(const std::shared_ptr<IHittable> &object) NOEXCEPT;

    void clear() NOEXCEPT;
    void add(const std::shared_ptr<IHittable> &object) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT;
};
