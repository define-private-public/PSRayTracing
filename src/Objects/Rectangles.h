#pragma once

#include "Interfaces/IHittable.h"
#include "AABB.h"
class IMaterial;


//*== Three types of axis-aligned rectangles

class XYRect : public IHittable {
private:
    // Data
    rreal _x0, _x1;
    rreal _y0, _y1;
    rreal _k;
    std::shared_ptr<IMaterial> _mat_ptr;

public:
    XYRect(
        const rreal x0,
        const rreal x1,
        const rreal y0,
        const rreal y1,
        const rreal k,
        std::shared_ptr<IMaterial> mat
    ) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};


class XZRect : public IHittable {
private:
    // Data
    rreal _x0, _x1;
    rreal _z0, _z1;
    rreal _k;
    std::shared_ptr<IMaterial> _mat_ptr;

public:
    XZRect(
        const rreal x0,
        const rreal x1,
        const rreal z0,
        const rreal z1,
        const rreal k,
        std::shared_ptr<IMaterial> mat
    ) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};


class YZRect : public IHittable {
private:
    // Data
    rreal _y0, _y1;
    rreal _z0, _z1;
    rreal _k;
    std::shared_ptr<IMaterial> _mat_ptr;

public:
    YZRect(
        const rreal y0,
        const rreal y1,
        const rreal z0,
        const rreal z1,
        const rreal k,
        std::shared_ptr<IMaterial> mat
    ) NOEXCEPT;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;
};
