#pragma once

#include "Interfaces/ITexture.h"
#include <memory>


class CheckerTexture : public ITexture {
private:
    // Data
    std::shared_ptr<ITexture> _even = nullptr;
    std::shared_ptr<ITexture> _odd = nullptr;

public:
    // Added by my so the user can tweak the fequency
    rreal x_frequency = 10;
    rreal y_frequency = 10;
    rreal z_frequency = 10;

public:
    explicit CheckerTexture(
        const std::shared_ptr<ITexture> &even,
        const std::shared_ptr<ITexture> &odd
    ) NOEXCEPT;
    explicit CheckerTexture(const Vec3 &even_clr, const Vec3 &odd_clr) NOEXCEPT;

    virtual std::shared_ptr<ITexture> deep_copy() const NOEXCEPT override;

    virtual Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
