#include "Materials/Dielectric.h"
#include <cmath>
#include "ScatterRecord.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Util.h"
#include "RandomGenerator.h"

using namespace std;


Dielectric::Dielectric(const rreal refractive_index) NOEXCEPT :
    _refractive_index(refractive_index)
{ }

shared_ptr<IMaterial> Dielectric::deep_copy() const NOEXCEPT {
    return make_shared<Dielectric>(*this);
}

bool Dielectric::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &h_rec,
    ScatterRecord &s_rec,
    const RenderMethod method
) const NOEXCEPT {
    s_rec.attenuation = Vec3(1.0);

    if (method == RenderMethod::Book3)
    {
        s_rec.is_specular = true;
        s_rec.pdf_ptr = nullptr;
    }

    const rreal etai_over_etat = h_rec.front_face ? (static_cast<rreal>(1) / _refractive_index) : _refractive_index;
    const Vec3 unit_direction = r_in.direction.unit_vector();
    const rreal cos_theta = std::fmin((-unit_direction).dot(h_rec.normal), static_cast<rreal>(1));
    const rreal sin_theta = util::sqrt(1 - (cos_theta * cos_theta));

#ifdef USE_BOOK_DIELECTRIC_SCATTER
    if ((etai_over_etat * sin_theta) > 1) {
        // Must reflect
        s_rec.ray = Ray(h_rec.p, unit_direction.reflect(h_rec.normal), r_in.time);
        return true;
    }

    // Check case for when at an angle a dielectric becomes mirrorlike
    const rreal reflect_probe = util::schlick(cos_theta, etai_over_etat);
    if (rng.get_real() < reflect_probe) {
        s_rec.ray = Ray(h_rec.p, unit_direction.reflect(h_rec.normal), r_in.time);
        return true;
    }

    // instead refract
    s_rec.ray = Ray(h_rec.p, unit_direction.refract(h_rec.normal, etai_over_etat), r_in.time);
#else
    // Like in many other sections, we are trying to have the compiler break up these computation
    // into vector instructions for us, and reduce the amounts of possible branching

    // Check for reflections
    const bool reflect_case_one = ((etai_over_etat * sin_theta) > 1);
    const rreal reflect_probe = util::schlick(cos_theta, etai_over_etat);
    const bool reflect_case_two = (rng.get_real() < reflect_probe);                 // This is a case whre the dielectric becomes mirrorlike
    const bool reflect = (reflect_case_one || reflect_case_two);

    // Choose either to reflect or refract
    const Vec3 reflect_dir = unit_direction.reflect(h_rec.normal);
    const Vec3 refract_dir = unit_direction.refract(h_rec.normal, etai_over_etat);
    const Vec3 direction = (reflect ? reflect_dir : refract_dir);

    s_rec.ray = Ray(h_rec.p, direction, r_in.time);
#endif

    return true;
}

rreal Dielectric::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const Ray &scattered
) const NOEXCEPT
    { return 0; }

Vec3 Dielectric::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return Vec3(0);
}
