#pragma once

#include "Interfaces/IMaterial.h"


// This is a material that shows the normal of a object's surface
class SurfaceNormal : public IMaterial {
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
        const Ray &r_in,        // In
        const HitRecord &rec,   // In
        ScatterRecord &s_rec    // Out
    ) const NOEXCEPT override;

    rreal scattering_pdf(
        const Ray &r_in,
        const HitRecord &rec,
        const Ray &scattered
    ) const NOEXCEPT override;

    Vec3 emitted(const Ray &r_in, const HitRecord &rec, const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
