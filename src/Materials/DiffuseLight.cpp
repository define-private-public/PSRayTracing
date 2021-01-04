#include "Materials/DiffuseLight.h"
#include "Textures/SolidColor.h"
#include "HitRecord.h"
#include "ScatterRecord.h"

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
    [[maybe_unused]] ScatterRecord &s_rec
) const NOEXCEPT {
    return false;
}

Vec3 DiffuseLight::emitted(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &rec,
    const rreal u,
    const rreal v,
    const Vec3 &p
) const NOEXCEPT {
    if (rec.front_face)
        return _emit->value(u, v, p);

    return Vec3(0);
}
