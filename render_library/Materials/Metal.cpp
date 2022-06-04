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
    const Ray &r_in,
    const HitRecord &h_rec,
    ScatterRecord &s_rec,
    const RenderMethod method
) const NOEXCEPT {
    const Vec3 reflected = r_in.direction.unit_vector().reflect(h_rec.normal);
    s_rec.ray = Ray(h_rec.p, reflected + (_fuzz * rng.get_in_unit_sphere()), r_in.time);
    s_rec.attenuation = _albedo;

    // By default metal will always scatter (i.e. for book 3)
    bool do_scatter = true;

    if (method == RenderMethod::Books1And2)
    {
        // Though in books 1 & 2, we check the reflected ray  vs. the hit normal
        do_scatter = (s_rec.ray.direction.dot(h_rec.normal) > 0);
    }
    else
    {
        s_rec.is_specular = true;
        s_rec.pdf_ptr = nullptr;
    }

    return do_scatter;
}

rreal Metal::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const Ray &scattered
) const NOEXCEPT
    { return 0; }

Vec3 Metal::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return Vec3(0);
}
