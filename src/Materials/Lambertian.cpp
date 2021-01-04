#include "Materials/Lambertian.h"
#include "HitRecord.h"
#include "ScatterRecord.h"
#include "Textures/SolidColor.h"
#include "RandomGenerator.h"
#include "ONB.h"
#include "PDFs/CosinePDF.h"

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
    [[maybe_unused]] RandomGenerator &rng,
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &rec,
    ScatterRecord &s_rec    // Out
) const NOEXCEPT {
    s_rec.is_specular = false;
    s_rec.attenuation = _albedo->value(rec.u, rec.v, rec.p);
    s_rec.pdf_ptr = make_shared<CosinePDF>(rec.normal);
    // TODO the above being a shared pointer is slow (and being always new allocated)

    return true;
}

rreal Lambertian::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    const HitRecord &rec,
    const Ray &scattered
) const NOEXCEPT {
    const rreal cosine = rec.normal.dot(scattered.direction.unit_vector());
    return (cosine < 0) ? 0 : (cosine / Pi);
}

Vec3 Lambertian::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {
    return Vec3(0);
}
