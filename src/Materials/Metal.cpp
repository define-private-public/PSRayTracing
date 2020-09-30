#include "Materials/Metal.h"
#include "Ray.h"
#include "HitRecord.h"
#include "RandomGenerator.h"


Metal::Metal(const Vec3 &albedo, const rreal fuzz) NOEXCEPT :
    _albedo(albedo),
    _fuzz(std::min(fuzz, static_cast<rreal>(1)))
{ }

bool Metal::scatter(
    RandomGenerator &rng,
    const Ray &r_in,        // In
    const HitRecord &rec,   // In
    Vec3 &attenuation,      // Out
    Ray &scattered          // Out
) const NOEXCEPT {
    const Vec3 reflected = r_in.direction.unit_vector().reflect(rec.normal);
    scattered = Ray(rec.p, reflected + (_fuzz * rng.get_in_unit_sphere()), r_in.time);
    attenuation = _albedo;

    return (scattered.direction.dot(rec.normal) > 0);
}

Vec3 Metal::emitted(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
