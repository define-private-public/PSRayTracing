#pragma once

#include "Interfaces/IMaterial.h"
#include <memory>
class ITexture;


class Lambertian : public IMaterial {
private:
    // Data
    std::shared_ptr<ITexture> _albedo;

public:
    explicit Lambertian(const Vec3 &solid_color) NOEXCEPT;
    explicit Lambertian(const std::shared_ptr<ITexture> &tex) NOEXCEPT;

    virtual bool scatter(
        RandomGenerator &rng,
        const Ray &r_in,        // In
        const HitRecord &rec,   // In
        Vec3 &attenuation,      // Out
        Ray &scattered          // Out
    ) const NOEXCEPT override;

    virtual Vec3 emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
