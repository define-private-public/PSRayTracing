#include "Materials/SurfaceNormal.h"
#include <cmath>
#include "HitRecord.h"
#include "RandomGenerator.h"

using namespace std;


SurfaceNormal::SurfaceNormal(const rreal brightness_, const rreal saturation_) NOEXCEPT :
    brightness(brightness_),
    saturation(saturation_)
{ }

bool SurfaceNormal::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &rec,
    Vec3 &attenuation,
    Ray &scattered
) const NOEXCEPT {
    const Vec3 scatter_direction = rec.normal + rng.get_unit_vector();
    const Vec3 a = (saturation * rec.normal) + Vec3(static_cast<rreal>(brightness));
    attenuation = Vec3(a.x * a.x, a.y * a.y, a.z * a.z);
    scattered = Ray(rec.p, scatter_direction, r_in.time);
    return true;
}

Vec3 SurfaceNormal::emitted(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
