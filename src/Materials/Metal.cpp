#include "Materials/Metal.h"
#include "Ray.h"
#include "HitRecord.h"
#include "ScatterRecord.h"
#include "RandomGenerator.h"

using namespace std;


Metal::Metal(const Vec3 &albedo, const rreal fuzz) NOEXCEPT :
    _albedo(albedo),
    _fuzz(std::min(fuzz, static_cast<rreal>(1)))
{ }

shared_ptr<IMaterial> Metal::deep_copy() const NOEXCEPT {
    return make_shared<Metal>(*this);
}

bool Metal::scatter(
    RandomGenerator &rng,
    const Ray &r_in,        // In
    const HitRecord &rec,   // In
    ScatterRecord &s_rec    // Out
) const NOEXCEPT {
    const Vec3 reflected = r_in.direction.unit_vector().reflect(rec.normal);
    s_rec.specular_ray = Ray(rec.p, reflected + (_fuzz * rng.get_in_unit_sphere()), r_in.time);
    s_rec.attenuation = _albedo;
    s_rec.is_specular = true;
    s_rec.pdf = monostate();

    return true;
}

Vec3 Metal::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
