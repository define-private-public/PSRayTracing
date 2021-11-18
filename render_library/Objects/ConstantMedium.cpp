#include "Objects/ConstantMedium.h"
#include <cmath>
#include "Materials/Isotropic.h"
#include "Interfaces/ITexture.h"
#include "RandomGenerator.h"

using namespace std;


ConstantMedium::ConstantMedium(
    const shared_ptr<IHittable> &boundary,
    const rreal density,
    const shared_ptr<ITexture> &phase
) NOEXCEPT :
    _boundary(boundary),
    _phase_function(make_shared<Isotropic>(phase)),
    _neg_inv_intensity(static_cast<rreal>(-1) / density)
{ }

ConstantMedium::ConstantMedium(
    const shared_ptr<IHittable> &boundary,
    const rreal density,
    const Vec3 &clr
) NOEXCEPT :
    _boundary(boundary),
    _phase_function(make_shared<Isotropic>(clr)),
    _neg_inv_intensity(static_cast<rreal>(-1) / density)
{ }

std::shared_ptr<IHittable> ConstantMedium::deep_copy() const NOEXCEPT {
    auto cm = make_shared<ConstantMedium>(*this);
    cm->_boundary = _boundary->deep_copy();
    cm->_phase_function = _phase_function->deep_copy();

    return cm;
}

bool ConstantMedium::hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT {
    HitRecord rec1, rec2;

    if (!_boundary->hit(rng, r, -Infinity, Infinity, rec1))
        return false;

    if (!_boundary->hit(rng, r, rec1.t + static_cast<rreal>(0.0001), Infinity, rec2))
        return false;

    rec1.t = max(rec1.t, t_min);
    rec2.t = min(rec2.t, t_max);

    if (rec1.t >= rec2.t)
        return false;

    rec1.t = max(rec1.t, static_cast<rreal>(0));

    const rreal ray_length = r.direction.length();
    const rreal distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const rreal hit_distance = _neg_inv_intensity * log(rng.get_real());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + (hit_distance / ray_length);
    rec.p = r.at(rec.t);

    rec.normal = Vec3(1, 0, 0); // Arbitrary
    rec.front_face = true;      // Also arbitrary
    rec.set_mat_ptr(_phase_function);

    return true;
}

bool ConstantMedium::bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT {
    return _boundary->bounding_box(t0, t1, output_box);
}
