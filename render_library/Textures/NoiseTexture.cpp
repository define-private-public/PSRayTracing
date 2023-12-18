#include "NoiseTexture.hpp"
#include "Perlin.hpp"
#include "PerlinReal.hpp"
#include "Util.hpp"

using namespace std;


NoiseTexture::NoiseTexture(const std::shared_ptr<Perlin> &noise) NOEXCEPT :
    _noise(noise)
{ }

NoiseTexture::NoiseTexture(const std::shared_ptr<Perlin> &noise, const rreal scale) NOEXCEPT :
    _scale(scale),
    _noise(noise)
{ }

NoiseTexture::NoiseTexture(const std::shared_ptr<PerlinReal> &noise) NOEXCEPT :
    _noise_real(noise)
{ }

NoiseTexture::NoiseTexture(const std::shared_ptr<PerlinReal> &noise, const rreal scale) NOEXCEPT :
    _scale(scale),
    _noise_real(noise)
{ }

shared_ptr<ITexture> NoiseTexture::deep_copy() const NOEXCEPT {
    // Deep copy all shared pointers
    auto nt = make_shared<NoiseTexture>(*this);

    // TODO: if using the Book's perlin implementation, I think this would fail since there is no compiler
    //       implemented copy constructors, test it!
    if (_noise)
        nt->_noise = make_shared<Perlin>(*_noise);
    if (_noise_real)
        nt->_noise_real = make_shared<PerlinReal>(*_noise_real);

    return nt;
}

Vec3 NoiseTexture::value(
        [[maybe_unused]] const rreal u,
        [[maybe_unused]] const rreal v,
        const Vec3 &p
) const NOEXCEPT {
    // First default to using the vector based perlin noise
    if (_noise) {
        switch (style) {
            case NoiseStyle::Normal:            return Vec3(0.5) * (1 + _noise->noise(_scale * p));    // Non-turbulent
            case NoiseStyle::Turbulent:         return Vec3(_noise->turb(_scale * p));
            case NoiseStyle::Marble:            return Vec3(0.5) * (1 + util::sin((_scale * p.z) + (10 * _noise->turb(p))));
            case NoiseStyle::Book2FinalScene:   return Vec3(0.5) * (1 + util::sin((_scale * p.x) + (5 * _noise->turb(_scale * p))));
        }

        // Use the non-turbulence version
    }

    // Default, use the `real` based noise
    return Vec3(_noise_real->noise(_scale * p));
}
