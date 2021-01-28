#pragma once

#include "Interfaces/IPDF.h"


// A PDF that mixes two others
class MixturePDF : public IPDF {
private:
    // Data
    IPDF *_a = nullptr;
    IPDF *_b = nullptr;

public:
    explicit MixturePDF(
        IPDF *a,
        IPDF *b
    ) NOEXCEPT :
        _a(a),
        _b(b)
    { }

    rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT override;
    Vec3 generate(RandomGenerator &rng) const NOEXCEPT override;
};
