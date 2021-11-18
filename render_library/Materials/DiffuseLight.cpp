#include "Materials/DiffuseLight.h"
#include "Textures/SolidColor.h"

using namespace std;


DiffuseLight::DiffuseLight(const shared_ptr<ITexture> &a) NOEXCEPT :
    _emit(a)
{ }

DiffuseLight::DiffuseLight(const Vec3 &clr) NOEXCEPT :
    _emit(make_shared<SolidColor>(clr))
{ }

shared_ptr<IMaterial> DiffuseLight::deep_copy() const NOEXCEPT {
    // Do deep copy
    auto dl = make_shared<DiffuseLight>(*this);
    dl->_emit = _emit->deep_copy();

    return dl;
}

bool DiffuseLight::scatter(
    [[maybe_unused]] RandomGenerator &rng,
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &rec,
    [[maybe_unused]] Vec3 &attenuation,
    [[maybe_unused]] Ray &scattered
) const NOEXCEPT {
    return false;
}

Vec3 DiffuseLight::emitted(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT {
    return _emit->value(u, v, p);
}
