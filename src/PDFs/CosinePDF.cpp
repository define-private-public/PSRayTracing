#include "PDFs/CosinePDF.h"
#include "RandomGenerator.h"

rreal CosinePDF::value(
    [[maybe_unused]] RandomGenerator &rng,
    const Vec3 &direction
) const NOEXCEPT {
    const rreal cosine = direction.unit_vector().dot(_uvw.w());
    return (cosine <= 0) ? 0 : (cosine / Pi);
}

Vec3 CosinePDF::generate(RandomGenerator &rng) const NOEXCEPT {
    return _uvw.local(rng.get_cosine_direction());
}
