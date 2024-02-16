#pragma once

#include "Interfaces/IMaterial.hpp"


// This is a material that shows the normal of a object's surface
class SurfaceNormal FINAL : public IMaterial {
public:
    // Data
    rreal brightness = static_cast<rreal>(0.6);
    rreal saturation = 0.75;

public:
    explicit SurfaceNormal() NOEXCEPT = default;
    explicit SurfaceNormal(const rreal brightness_, const rreal saturation_) NOEXCEPT;

    std::shared_ptr<IMaterial> deep_copy() const NOEXCEPT override;

    bool scatter(
        RandomGenerator &rng,
        const Ray &r_in,
        const HitRecord &h_rec,
        ScatterRecord &s_rec,
        const RenderMethod method
    ) const NOEXCEPT override;

    rreal scattering_pdf(
        const Ray &r_in,
        const HitRecord &h_rec,
        const Ray &scattered
    ) const NOEXCEPT override;

    Vec3 emitted(
        const Ray &r_in,
        const HitRecord &h_rec,
        const rreal u,
        const rreal v,
        const Vec3 &p,
        const RenderMethod method
    ) const NOEXCEPT override;
};
