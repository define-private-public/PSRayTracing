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
