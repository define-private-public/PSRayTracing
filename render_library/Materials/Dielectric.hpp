#pragma once

#include "Interfaces/IMaterial.hpp"


// E.g. a glass like (refracting) material
class Dielectric FINAL : public IMaterial {
public:
    // Data
    rreal _refractive_index;

public:
    explicit Dielectric(const rreal refractive_index) NOEXCEPT;

    std::shared_ptr<IMaterial> deep_copy() const NOEXCEPT override;

    bool scatter(
        RandomGenerator &rng,
        const Ray &r_in,
        const HitRecord &h_rec,
        ScatterRecord &s_rec,
        const RenderMethod method
    ) const NOEXCEPT override;

    virtual rreal scattering_pdf(
        const Ray &r_in,
        const HitRecord &h_rec,
        const Ray &scattered
    ) const NOEXCEPT override;

    virtual Vec3 emitted(
        const Ray &r_in,
        const HitRecord &h_rec,
        const rreal u,
        const rreal v,
        const Vec3 &p,
        const RenderMethod method
    ) const NOEXCEPT override;
};
