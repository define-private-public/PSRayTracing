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
