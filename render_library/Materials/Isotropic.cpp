#include "Materials/Isotropic.h"
#include "Textures/SolidColor.h"
#include "HitRecord.h"
#include "ScatterRecord.h"
#include "RandomGenerator.h"
using namespace std;


Isotropic::Isotropic(const Vec3 &clr) NOEXCEPT :
    _albedo(make_shared<SolidColor>(clr))
{ }

Isotropic::Isotropic(const shared_ptr<ITexture> &albedo) NOEXCEPT :
    _albedo(albedo)
{ }

shared_ptr<IMaterial> Isotropic::deep_copy() const NOEXCEPT {
    // Do deep copy
    auto iso = make_shared<Isotropic>(*this);
    iso->_albedo = _albedo->deep_copy();

    return iso;
}

bool Isotropic::scatter(
    RandomGenerator &rng,
    const Ray &r_in,
    const HitRecord &h_rec,
    ScatterRecord &s_rec,
    const RenderMethod method
) const NOEXCEPT {
    s_rec.ray = Ray(h_rec.p, rng.get_in_unit_sphere(), r_in.time);
    s_rec.attenuation = _albedo->value(h_rec.u, h_rec.v, h_rec.p);

    if (method == RenderMethod::Book3)
    {
        // Note: This wasn't in the book's code, so I had to add it here.
        //
        //       If you look at the github for the book 3's reference code, you'll see they just `#if 0 ... #endf`
        //       the entire `Isotropic::scatter()` method.
        s_rec.is_specular = false;

        #ifdef USE_BOOK_PDF_MANAGEMENT
            s_rec.pdf_ptr = nullptr;        // TODO should this be a Cosine PDF?
        #else
            s_rec.pdf = monostate();
        #endif
    }

    return true;
}

rreal Isotropic::scattering_pdf(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const Ray &scattered
) const NOEXCEPT
    { return 0; }

Vec3 Isotropic::emitted(
    [[maybe_unused]] const Ray &r_in,
    [[maybe_unused]] const HitRecord &h_rec,
    [[maybe_unused]] const rreal u,
    [[maybe_unused]] const rreal v,
    [[maybe_unused]] const Vec3 &p,
    [[maybe_unused]] const RenderMethod method
) const NOEXCEPT {
    return Vec3(0);
}
