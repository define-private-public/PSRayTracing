#pragma once

#include "Interfaces/IMaterial.h"
#include <memory>
class ITexture;


class Isotropic : public IMaterial {
private:
    // Data
    std::shared_ptr<ITexture> _albedo;

public:
    explicit Isotropic(const Vec3 &clr) NOEXCEPT;
    explicit Isotropic(const std::shared_ptr<ITexture> &albedo) NOEXCEPT;

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
