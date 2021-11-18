#pragma once

#include "Vec3.h"

// A simple pixel (represented by floats)

struct ColorRGBA {
    rreal r = 0;
    rreal g = 0;
    rreal b = 0;
    rreal a = 1;             // Starts opaque


    inline static ColorRGBA from_Vec3(const Vec3 &v, const rreal alpha=1) NOEXCEPT {
        ColorRGBA clr;
        clr.r = v.x;
        clr.g = v.y;
        clr.b = v.z;
        clr.a = alpha;

        return clr;
    }
};
