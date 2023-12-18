#include "PDFs/HittablePDF.hpp"
#include "RandomGenerator.hpp"

rreal HittablePDF::value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT {
    // This is slighty different from the book, to handle a case where a `null` was provided
    if (_obj)
        return _obj->pdf_value(rng, _origin, direction);

    return 0;
}

Vec3 HittablePDF::generate(RandomGenerator &rng) const NOEXCEPT {
    // This is slighty different from the book, to handle a case where a `null` was provided
    if (_obj)
        return _obj->random(rng, _origin);

    return Vec3(1, 0, 0);
}
