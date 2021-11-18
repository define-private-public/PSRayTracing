#include "Materials/Lambertian.h"
#include "HitRecord.h"
#include "Textures/SolidColor.h"
#include "RandomGenerator.h"

using namespace std;


Lambertian::Lambertian(const Vec3 &solid_color) NOEXCEPT :
    Lambertian(make_shared<SolidColor>(solid_color))
{ }

Lambertian::Lambertian(const shared_ptr<ITexture> &tex) NOEXCEPT :
    _albedo(tex)
{ }

shared_ptr<IMaterial> Lambertian::deep_copy() const NOEXCEPT {
    // Do deep copy
    auto lamb = make_shared<Lambertian>(*this);
    lamb->_albedo = _albedo->deep_copy();

    return lamb;
}

bool Lambertian::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &rec,
    Vec3 &attenuation,
    Ray &scattered
) const NOEXCEPT {
    const Vec3 scatter_direction = rec.normal + rng.get_unit_vector();
    scattered = Ray(rec.p, scatter_direction, r_in.time);
    attenuation = _albedo->value(rec.u, rec.v, rec.p);

    return true;
}

Vec3 Lambertian::emitted(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
