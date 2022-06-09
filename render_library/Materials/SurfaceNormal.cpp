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
    const HitRecord &h_rec,
    ScatterRecord &s_rec,
    const RenderMethod method
) const NOEXCEPT {
    const Vec3 scatter_direction = h_rec.normal + rng.get_unit_vector();
    const Vec3 a = (saturation * h_rec.normal) + Vec3(static_cast<rreal>(brightness));

    s_rec.attenuation = Vec3(a.x * a.x, a.y * a.y, a.z * a.z);
    s_rec.ray = Ray(h_rec.p, scatter_direction, r_in.time);

    if (method == RenderMethod::Book3)
    {
        // Since this material is very close to Lambertian, we use a cosine PDF
        s_rec.is_specular = false;

        #ifdef USE_BOOK_PDF_MANAGEMENT
            s_rec.pdf_ptr = make_shared<CosinePDF>(h_rec.normal);
        #else
            s_rec.pdf = CosinePDF(h_rec.normal);
        #endif
    }

    return true;
}

rreal SurfaceNormal::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &h_rec,
    const Ray &scattered
) const NOEXCEPT {
    // Since this material is very close to a Lambertian, we use a cosine PDF
    const rreal cosine = h_rec.normal.dot(scattered.direction.unit_vector());
    return (cosine < 0) ? 0 : (cosine / Pi);
}

Vec3 SurfaceNormal::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return Vec3(0);
}
