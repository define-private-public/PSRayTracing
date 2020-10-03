#pragma once

#include "Interfaces/ICamera.h"
#include "Vec3.h"


// A standard postionable camera with focus (and blur)
class Camera : public ICamera {
protected:
    // Data
    Vec3 _origin;
    Vec3 _lower_left_corner;
    Vec3 _horizontal;
    Vec3 _vertical;
    Vec3 _u, _v, _w;      // Used for camera's local coordinates and determine the focal planes
    rreal _lens_radius;

public:
    explicit Camera(
        const Vec3 &look_from,
        const Vec3 &look_at,
        const Vec3 &v_up,
        const rreal vfov,            // Vertical field of view
        const rreal aspect_ratio,
        const rreal aperature,
        const rreal focus_dist
    ) NOEXCEPT;

    virtual Ray get_ray(RandomGenerator &rng, const rreal s, const rreal t) const NOEXCEPT override;
};
