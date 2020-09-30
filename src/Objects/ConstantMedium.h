#pragma once

#include "Interfaces/IHittable.h"
class IMaterial;
class ITexture;


class ConstantMedium : public IHittable {
private:
    // Data
    std::shared_ptr<IHittable> _boundary = nullptr;
    std::shared_ptr<IMaterial> _phase_function = nullptr;
    rreal _neg_inv_intensity = 0;

public:
    ConstantMedium(
        const std::shared_ptr<IHittable> &boundary,
        const rreal density,
        const std::shared_ptr<ITexture> &phase
    ) NOEXCEPT;

    ConstantMedium(
        const std::shared_ptr<IHittable> &boundary,
        const rreal density,
        const Vec3 &clr
    ) NOEXCEPT;

    bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;

};
