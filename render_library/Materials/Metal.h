#pragma once

#include "Interfaces/IMaterial.h"


class Metal : public IMaterial {
public:
    // Data
    Vec3 _albedo;
    rreal _fuzz;

public:
    explicit Metal(const Vec3 &albedo, const rreal fuzz) NOEXCEPT;

    std::shared_ptr<IMaterial> deep_copy() const NOEXCEPT override;

    bool scatter(
        RandomGenerator &rng,
        const Ray &r_in,        // In
        const HitRecord &rec,   // In
        Vec3 &attenuation,      // Out
        Ray &scattered          // Out
    ) const NOEXCEPT override;

    Vec3 emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
