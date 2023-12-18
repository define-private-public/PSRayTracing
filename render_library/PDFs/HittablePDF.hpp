#pragma once

#include "Interfaces/IPDF.hpp"
#include "Interfaces/IHittable.hpp"


class HittablePDF : public IPDF {
private:
    // Data
    Vec3 _origin;
    std::shared_ptr<IHittable> _obj;

public:
    explicit HittablePDF(const std::shared_ptr<IHittable> &object, Vec3 &origin) NOEXCEPT :
        _origin(origin),
        _obj(object)
    { }

    rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT override;
    Vec3 generate(RandomGenerator &rng) const NOEXCEPT override;
};
