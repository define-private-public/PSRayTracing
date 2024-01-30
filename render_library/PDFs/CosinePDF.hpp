#pragma once

#include "Interfaces/IPDF.hpp"
#include "ONB.hpp"


class CosinePDF FINAL : public IPDF {
private:
    // Data
    ONB _uvw;

public:
    explicit CosinePDF(const Vec3 &w) NOEXCEPT {
        _uvw.build_from_w(w);
    }

    rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT override;
    Vec3 generate(RandomGenerator &rng) const NOEXCEPT override;
};
