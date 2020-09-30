#pragma once

#include "Interfaces/ITexture.h"
#include <memory>
class PerlinReal;
class Perlin;


// TODO [feature]
// - coloured noise?  right now it's only return a greyscale.
//   we could call the `noise.noise()` function three times and get something different

class NoiseTexture : public ITexture {
public:
    enum class NoiseStyle {
        Normal = 0,
        Turbulent,
        Marble,
        Book2FinalScene,
    };

private:
    // Data
    rreal _scale = 1;
    std::shared_ptr<Perlin> _noise = nullptr;       // Can only be one of these two
    std::shared_ptr<PerlinReal> _noise_real = nullptr;

public:
    // options for regular `Perlin`
    NoiseStyle style = NoiseStyle::Marble;

    NoiseTexture(const std::shared_ptr<Perlin> &noise) NOEXCEPT;
    NoiseTexture(const std::shared_ptr<Perlin> &noise, const rreal scale) NOEXCEPT;
    NoiseTexture(const std::shared_ptr<PerlinReal> &noise) NOEXCEPT;
    NoiseTexture(const std::shared_ptr<PerlinReal> &noise, const rreal scale) NOEXCEPT;

    virtual Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
