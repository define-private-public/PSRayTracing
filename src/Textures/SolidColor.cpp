#include "SolidColor.h"


Vec3 SolidColor::value(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return _colour_value;
}
