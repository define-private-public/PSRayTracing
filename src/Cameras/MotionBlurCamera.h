#pragma once

#include "Camera.h"


// A camera that supports motion blue
class MotionBlurCamera : public Camera {
private:
    // Data
    rreal _time0, _time1;        // Shutter open/close times

public:
    explicit MotionBlurCamera(
        const Vec3 &look_from,
        const Vec3 &look_at,
        const Vec3 &v_up,
        const rreal vfov,            // Vertical field of view
        const rreal aspect_ratio,
        const rreal aperature,
        const rreal focus_dist,
        const rreal t0, const rreal t1
    ) NOEXCEPT;

    virtual std::shared_ptr<ICamera> deep_copy() const NOEXCEPT override;

    virtual Ray get_ray(RandomGenerator &rng, const rreal s, const rreal t) const NOEXCEPT override;
};
