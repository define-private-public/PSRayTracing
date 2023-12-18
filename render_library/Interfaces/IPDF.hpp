#pragma once

#include "Vec3.hpp"
class RandomGenerator;


// Interface for defining a Probabily Distribution Function
class IPDF {
public:
    virtual ~IPDF() NOEXCEPT = default;

    virtual rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT = 0;
    virtual Vec3 generate(RandomGenerator &rng) const NOEXCEPT = 0;
};
