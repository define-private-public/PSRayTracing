#pragma once

#include "Vec3.h"
#include "Interfaces/IDeepCopyable.h"

class ITexture : public IDeepCopyable<ITexture> {
public:
    virtual ~ITexture() NOEXCEPT = default;

    virtual std::shared_ptr<ITexture> deep_copy() const NOEXCEPT = 0;

    virtual Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT = 0;
};
