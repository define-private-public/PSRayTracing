#include "Materials/Dielectric.h"
#include <cmath>
#include "HitRecord.h"
#include "Ray.h"
#include "Util.h"
#include "RandomGenerator.h"


Dielectric::Dielectric(const rreal refractive_index) NOEXCEPT :
    _refractive_index(refractive_index)
{ }

bool Dielectric::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &rec,
    Vec3 &attenuation,
    Ray &scattered
) const NOEXCEPT {
    attenuation = Vec3(1.0);
    const rreal etai_over_etat = rec.front_face ? (static_cast<rreal>(1) / _refractive_index) : _refractive_index;
    const Vec3 unit_direction = r_in.direction.unit_vector();
    const rreal cos_theta = std::fmin((-unit_direction).dot(rec.normal), static_cast<rreal>(1));
    const rreal sin_theta = util::sqrt(1 - (cos_theta * cos_theta));

#ifdef USE_BOOK_DIELECTRIC_SCATTER
    if ((etai_over_etat * sin_theta) > 1) {
        // Must reflect
        scattered = Ray(rec.p, unit_direction.reflect(rec.normal), r_in.time);
        return true;
    }

    // Check case for when at an angle a dielectric becomes mirrorlike
    const rreal reflect_probe = util::schlick(cos_theta, etai_over_etat);
    if (rng.get_real() < reflect_probe) {
        scattered = Ray(rec.p, unit_direction.reflect(rec.normal), r_in.time);
        return true;
    }

    // instead refract
    scattered = Ray(rec.p, unit_direction.refract(rec.normal, etai_over_etat), r_in.time);
#else
    // Like in many other sections, we are trying to have the compiler break up these computation
    // into vector instructions for us, and reduce the amounts of possible branching

    // Check for reflections
    const bool reflect_case_one = (etai_over_etat * sin_theta) > 1;
    const rreal reflect_probe = util::schlick(cos_theta, etai_over_etat);
    const bool reflect_case_two = rng.get_real() < reflect_probe;           // This is a case whre the dielectric becomes mirrorlike

    const Vec3 reflect_dir = unit_direction.reflect(rec.normal);
    const Vec3 refract_dir = unit_direction.refract(rec.normal, etai_over_etat);

    if (reflect_case_one || reflect_case_two)
        scattered = Ray(rec.p, reflect_dir, r_in.time);
    else
        scattered = Ray(rec.p, refract_dir, r_in.time);     // Instead refract
#endif

    return true;
}

Vec3 Dielectric::emitted(
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
