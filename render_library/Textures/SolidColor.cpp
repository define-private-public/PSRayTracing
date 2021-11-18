#include "SolidColor.h"

using namespace std;


Vec3 SolidColor::value(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return _colour_value;
}

shared_ptr<ITexture> SolidColor::deep_copy() const NOEXCEPT {
    return make_shared<SolidColor>(*this);
}
