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
