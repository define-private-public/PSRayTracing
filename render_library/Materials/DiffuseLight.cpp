#include "Materials/DiffuseLight.hpp"
#include "HitRecord.hpp"
#include "ScatterRecord.hpp"
#include "Textures/SolidColor.hpp"

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
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] ScatterRecord &s_rec,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return false;
}

rreal DiffuseLight::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const Ray &scattered
) const NOEXCEPT
    { return 0; }

Vec3 DiffuseLight::emitted(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &h_rec,
    const rreal u,
    const rreal v,
    const Vec3 &p,
    const RenderMethod method
) const NOEXCEPT {
    Vec3 emission(0);   // Default emission is pitch black

    if (method == RenderMethod::Books1And2)
    {
        emission = _emit->value(u, v, p);
    }
    else
    {
        // Book 3
        if (h_rec.front_face)
            emission = _emit->value(u, v, p);
    }

    return emission;
}
