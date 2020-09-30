#pragma once

#include "Common.h"

class Ray;
class RandomGenerator;

// An interface so we can have multiple types of cameras
class ICamera {
public:
    virtual Ray get_ray(RandomGenerator &rng, const rreal s, const rreal t) const NOEXCEPT = 0;
};
