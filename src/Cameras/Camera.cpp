#include "Camera.h"
#include <cmath>
#include "Util.h"
#include "Ray.h"
#include "RandomGenerator.h"

using namespace util;


Camera::Camera(
    const Vec3 &look_from, const Vec3 &look_at, const Vec3 &v_up,
    const rreal vfov, const rreal aspect_ratio,
    const rreal aperature,
    const rreal focus_dist
) NOEXCEPT {
    const rreal theta = degrees_to_radians(vfov);
    const rreal h = std::tan(theta / 2);
    const rreal viewport_height = 2 * h;
    const rreal viewport_width = aspect_ratio * viewport_height;

    _w = (look_from - look_at).unit_vector();
    _u = v_up.cross(_w).unit_vector();
    _v = _w.cross(_u);

    _origin = look_from;
    _horizontal = focus_dist * viewport_width * _u;
    _vertical = focus_dist * viewport_height * _v;
    _lower_left_corner = _origin - (_horizontal / static_cast<rreal>(2)) - (_vertical / static_cast<rreal>(2)) - (focus_dist *_w);

    _lens_radius = aperature / 2;
}

Ray Camera::get_ray(RandomGenerator &rng, const rreal s, const rreal t) const NOEXCEPT {
    const Vec3 rd = _lens_radius * rng.get_in_unit_disk();
    const Vec3 offset = (_u * rd.x) + (_v * rd.y);

    const Vec3 ray_dir = _lower_left_corner
                       + (s * _horizontal)
                       + (t * _vertical)
                       - _origin - offset;

    return Ray(_origin + offset, ray_dir);
}
