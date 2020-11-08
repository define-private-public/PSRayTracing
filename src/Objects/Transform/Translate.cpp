#include "Objects/Transform/Translate.h"
#include "AABB.h"

using namespace std;


Translate::Translate(const shared_ptr<IHittable> &obj, const Vec3 &displacement) NOEXCEPT :
    _obj(obj),
    _offset(displacement)
{ }

std::shared_ptr<IHittable> Translate::deep_copy() const NOEXCEPT {
    auto trans = make_shared<Translate>(*this);
    trans->_obj = _obj->deep_copy();

    return trans;
}

bool Translate::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
    const Ray moved_r(r.origin - _offset, r.direction, r.time);
    if (!_obj->hit(rng, moved_r, t_min, t_max, rec))
        return false;

    rec.p += _offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool Translate::bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT {
    if (!_obj->bounding_box(t0, t1, output_box))
        return false;

    output_box = AABB(
        output_box.min + _offset,
        output_box.max + _offset
    );

    return true;
}
