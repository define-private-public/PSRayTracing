#pragma once

#include "Interfaces/ITexture.hpp"


class SolidColor : public ITexture {
private:
    // Data
    Vec3 _colour_value = Vec3(0);   // Default is black

public:
    explicit SolidColor(const Vec3 &c) NOEXCEPT :
        _colour_value(c)
    { }

    explicit SolidColor(const rreal red, const rreal green, const rreal blue) NOEXCEPT :
        SolidColor(Vec3(red, green, blue))
    { }

    std::shared_ptr<ITexture> deep_copy() const NOEXCEPT override;

    Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
