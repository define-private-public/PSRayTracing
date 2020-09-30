#include "Materials/Isotropic.h"
#include "Textures/SolidColor.h"
#include "HitRecord.h"
#include "RandomGenerator.h"
using namespace std;


Isotropic::Isotropic(const Vec3 &clr) NOEXCEPT :
    _albedo(make_shared<SolidColor>(clr))
{ }

Isotropic::Isotropic(const shared_ptr<ITexture> &albedo) NOEXCEPT :
    _albedo(albedo)
{ }

bool Isotropic::scatter(
    RandomGenerator &rng,
    const Ray &r_in,        // In
    const HitRecord &rec,   // In
    Vec3 &attenuation,      // Out
    Ray &scattered          // Out
) const NOEXCEPT {
    scattered = Ray(rec.p, rng.get_in_unit_sphere(), r_in.time);
    attenuation = _albedo->value(rec.u, rec.v, rec.p);

    return true;
}

Vec3 Isotropic::emitted(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
