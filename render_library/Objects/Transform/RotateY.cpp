#include "Objects/Transform/RotateY.h"
#include "Util.h"

using namespace std;


RotateY::RotateY(const shared_ptr<IHittable> &obj, const rreal angle) NOEXCEPT :
    _obj(obj)
{
    const rreal radians = util::degrees_to_radians(angle);
    _sin_theta = util::sin(radians);
    _cos_theta = util::cos(radians);
    _has_box = _obj->bounding_box(0, 1, _bbox);

    Vec3 lower(Infinity);
    Vec3 upper(-Infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                const auto ri = static_cast<rreal>(i);
                const auto rj = static_cast<rreal>(j);
                const auto rk = static_cast<rreal>(k);

                const rreal x = (ri * _bbox.max.x) + ((1 - ri) * _bbox.min.x);
                const rreal y = (rj * _bbox.max.y) + ((1 - rj) * _bbox.min.y);
                const rreal z = (rk * _bbox.max.z) + ((1 - rk) * _bbox.min.z);

                const rreal x_prime =  (_cos_theta * x) + (_sin_theta * z);
                const rreal z_prime = (-_sin_theta * x) + (_cos_theta * z);

                const Vec3 tester(x_prime, y, z_prime);

                // Have to expand things out here compared to the original book code since I don't use elements
                lower.x = min(lower.x, tester.x);
                upper.x = max(upper.x, tester.x);
                lower.y = min(lower.y, tester.y);
                upper.y = max(upper.y, tester.y);
                lower.z = min(lower.z, tester.z);
                upper.z = max(upper.z, tester.z);
            }
        }
    }

    _bbox = AABB(lower, upper);
}

std::shared_ptr<IHittable> RotateY::deep_copy() const NOEXCEPT {
    auto trans = make_shared<RotateY>(*this);
    trans->_obj = _obj->deep_copy();

    return trans;
}

bool RotateY::hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT {
    // Need to modify the direction and orientation of the ray

    Vec3 o_prime(
        (_cos_theta * r.origin.x) - (_sin_theta * r.origin.z),
        r.origin.y,
        (_sin_theta * r.origin.x) + (_cos_theta * r.origin.z)
    );

    Vec3 d_prime(
        (_cos_theta * r.direction.x) - (_sin_theta * r.direction.z),
        r.direction.y,
        (_sin_theta * r.direction.x) + (_cos_theta * r.direction.z)
    );

    Ray r_rotated(o_prime, d_prime, r.time);

    // Hit check
    if (!_obj->hit(rng, r_rotated, t_min, t_max, rec))
        return false;

    // Now need to rotate the hit record
    Vec3 p_prime(
        ( _cos_theta * rec.p.x) + (_sin_theta * rec.p.z),
        rec.p.y,
        (-_sin_theta * rec.p.x) + (_cos_theta * rec.p.z)
    );

    Vec3 n_prime(
        ( _cos_theta * rec.normal.x) + (_sin_theta * rec.normal.z),
        rec.normal.y,
        (-_sin_theta * rec.normal.x) + (_cos_theta * rec.normal.z)
    );

    rec.p = p_prime;
    rec.set_face_normal(r_rotated, n_prime);

    return true;
}

bool RotateY::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = _bbox;
    return _has_box;
}
