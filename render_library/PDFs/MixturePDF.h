#pragma once

#include "Interfaces/IPDF.h"
#include <memory>


// A PDF that mixes two others
class MixturePDF : public IPDF {
private:
    // Data
    std::shared_ptr<IPDF> _a;
    std::shared_ptr<IPDF> _b;

public:
    explicit MixturePDF(
        const std::shared_ptr<IPDF> &a,
        const std::shared_ptr<IPDF> &b
    ) NOEXCEPT :
        _a(a),
        _b(b)
    { }

    rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT override;
    Vec3 generate(RandomGenerator &rng) const NOEXCEPT override;
};
