#include "PDFs/MixturePDF.hpp"
#include "RandomGenerator.hpp"


rreal MixturePDF::value(
    RandomGenerator &rng,
    const Vec3 &direction
) const NOEXCEPT {
    // This is different from the book, as this handles a case where a PDF could be null
    const rreal a_val = _a ? (0.5 * _a->value(rng, direction)) : 0;
    const rreal b_val = _b ? (0.5 * _b->value(rng, direction)) : 0;

    return a_val + b_val;
}

Vec3 MixturePDF::generate(RandomGenerator &rng) const NOEXCEPT {
    // This is different from the book, as this handles a case where a PDF could be null
    const bool use_a = (rng.get_real() < 0.5);

    if (use_a)
        return _a ? _a->generate(rng) : Vec3(1, 0, 0);

    return _b ? _b->generate(rng) : Vec3(1, 0, 0);
}
