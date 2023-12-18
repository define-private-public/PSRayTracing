#include "Materials/Lambertian.hpp"
#include "HitRecord.hpp"
#include "ScatterRecord.hpp"
#include "Textures/SolidColor.hpp"
#include "RandomGenerator.hpp"
#include "ONB.hpp"
#include "PDFs/CosinePDF.hpp"

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
    const HitRecord &h_rec,
    ScatterRecord &s_rec,
    const RenderMethod method
) const NOEXCEPT {
    if (method == RenderMethod::Books1And2)
    {
        const Vec3 scatter_direction = h_rec.normal + rng.get_unit_vector();
        s_rec.ray = Ray(h_rec.p, scatter_direction, r_in.time);
    }
    else
    {
        // Book 3
        s_rec.is_specular = false;

        #ifdef USE_BOOK_PDF_MANAGEMENT
            s_rec.pdf_ptr = make_shared<CosinePDF>(h_rec.normal);
        #else
            s_rec.pdf = CosinePDF(h_rec.normal);
        #endif
    }

    s_rec.attenuation = _albedo->value(h_rec.u, h_rec.v, h_rec.p);

    return true;
}

rreal Lambertian::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &h_rec,
    const Ray &scattered
) const NOEXCEPT {
    const rreal cosine = h_rec.normal.dot(scattered.direction.unit_vector());
    return (cosine < 0) ? 0 : (cosine / Pi);
}

Vec3 Lambertian::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return Vec3(0);
}
