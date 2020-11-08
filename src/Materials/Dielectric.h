#pragma once

#include "Interfaces/IMaterial.h"


// E.g. a glass like (refracting) material
class Dielectric : public IMaterial {
public:
    // Data
    rreal _refractive_index;

public:
    explicit Dielectric(const rreal refractive_index) NOEXCEPT;

    virtual std::shared_ptr<IMaterial> deep_copy() const NOEXCEPT override;

    virtual bool scatter(
        RandomGenerator &rng,
        const Ray &r_in,        // In
        const HitRecord &rec,   // In
        Vec3 &attenuation,      // Out
        Ray &scattered          // Out
    ) const NOEXCEPT override;

    virtual Vec3 emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
