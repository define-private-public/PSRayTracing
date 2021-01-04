#include "Materials/SurfaceNormal.h"
#include <cmath>
#include "HitRecord.h"
#include "ScatterRecord.h"
#include "RandomGenerator.h"
#include "PDFs/CosinePDF.h"

using namespace std;


SurfaceNormal::SurfaceNormal(const rreal brightness_, const rreal saturation_) NOEXCEPT :
    brightness(brightness_),
    saturation(saturation_)
{ }

shared_ptr<IMaterial> SurfaceNormal::deep_copy() const NOEXCEPT {
    return make_shared<SurfaceNormal>(*this);
}

bool SurfaceNormal::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &rec,
    ScatterRecord &s_rec
) const NOEXCEPT {
    const Vec3 scatter_direction = rec.normal + rng.get_unit_vector();
    const Vec3 a = (saturation * rec.normal) + Vec3(static_cast<rreal>(brightness));

    // Since this material is very close to Lambertian, we use a cosine PDF
    s_rec.attenuation = Vec3(a.x * a.x, a.y * a.y, a.z * a.z);
    s_rec.specular_ray = Ray(rec.p, scatter_direction, r_in.time);
    s_rec.is_specular = false;
    s_rec.pdf_ptr = make_shared<CosinePDF>(rec.normal);

    return true;
}

// Since this material is very close to Lambertian, we use a cosine PDF
rreal SurfaceNormal::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &rec,
    const Ray &scattered
) const NOEXCEPT {
    rreal cosine = rec.normal.dot(scattered.direction.unit_vector());
    return (cosine < 0) ? 0 : (cosine / Pi);
}

Vec3 SurfaceNormal::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}