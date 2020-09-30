#pragma once

#include "Interfaces/IMaterial.h"
#include "Interfaces/ITexture.h"
#include <memory>


class DiffuseLight : public IMaterial {
private:
    // Data
    std::shared_ptr<ITexture> _emit;

public:
    DiffuseLight(const std::shared_ptr<ITexture> &a) NOEXCEPT;
    DiffuseLight(const Vec3 &clr) NOEXCEPT;

    virtual bool scatter(RandomGenerator &rng, const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const NOEXCEPT override;
    virtual Vec3 emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
