#include "Materials/Isotropic.h"
#include "Textures/SolidColor.h"
#include "HitRecord.h"
#include "ScatterRecord.h"
#include "RandomGenerator.h"
using namespace std;


Isotropic::Isotropic(const Vec3 &clr) NOEXCEPT :
    _albedo(make_shared<SolidColor>(clr))
{ }

Isotropic::Isotropic(const shared_ptr<ITexture> &albedo) NOEXCEPT :
    _albedo(albedo)
{ }

shared_ptr<IMaterial> Isotropic::deep_copy() const NOEXCEPT {
    // Do deep copy
    auto iso = make_shared<Isotropic>(*this);
    iso->_albedo = _albedo->deep_copy();

    return iso;
}

bool Isotropic::scatter(
    RandomGenerator &rng,
    const Ray &r_in,        // In
    const HitRecord &rec,   // In
    ScatterRecord &s_rec    // Out
) const NOEXCEPT {
    // Note: this wasn't in the book's code, so I had to add it here
    s_rec.specular_ray = Ray(rec.p, rng.get_in_unit_sphere(), r_in.time);
    s_rec.attenuation = _albedo->value(rec.u, rec.v, rec.p);
    s_rec.is_specular = false;
    s_rec.pdf_ptr = nullptr;        // TODO should this be a Cosine PDF?

    return true;
}

Vec3 Isotropic::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
