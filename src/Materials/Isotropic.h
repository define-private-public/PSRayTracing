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
        ScatterRecord &s_rec    // Out
    ) const NOEXCEPT override;

    rreal scattering_pdf(
        [[maybe_unused]] const Ray &r_in,
        [[maybe_unused]] const HitRecord &rec,
        [[maybe_unused]] const Ray &scattered
    ) const NOEXCEPT override {
        return 0;
    }

    Vec3 emitted(const Ray &r_in, const HitRecord &rec, const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
