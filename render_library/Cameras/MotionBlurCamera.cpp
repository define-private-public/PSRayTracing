#include "MotionBlurCamera.hpp"
#include <cmath>
#include "RandomGenerator.hpp"
#include "Ray.hpp"


MotionBlurCamera::MotionBlurCamera(
    const Vec3 &look_from,
    const Vec3 &look_at,
    const Vec3 &v_up,
    const rreal vfov,
    const rreal aspect_ratio,
    const rreal aperature,
    const rreal focus_dist,
    const rreal t0, const rreal t1
) NOEXCEPT :
    Camera(look_from, look_at, v_up, vfov, aspect_ratio, aperature, focus_dist),
    _time0(t0), _time1(t1)
{ }

std::shared_ptr<ICamera> MotionBlurCamera::deep_copy() const NOEXCEPT  {
    return std::make_shared<MotionBlurCamera>(*this);
}

Ray MotionBlurCamera::get_ray(RandomGenerator &rng, const rreal s, const rreal t) const NOEXCEPT {
    const Vec3 rd = _lens_radius * rng.get_in_unit_disk();
    const Vec3 offset = (_u * rd.x) + (_v * rd.y);

    const Vec3 ray_dir = _lower_left_corner
                       + (s * _horizontal)
                       + (t * _vertical)
                       - _origin - offset;

    return Ray(_origin + offset, ray_dir, rng.get_real(_time0, _time1));
}
