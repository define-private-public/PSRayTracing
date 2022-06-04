#include "FlipFace.h"

using namespace std;


shared_ptr<IHittable> FlipFace::deep_copy() const NOEXCEPT {
    return make_shared<FlipFace>(_obj->deep_copy());
}

bool FlipFace::hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT {
    if (!_obj->hit(rng, r, t_min, t_max, rec))
        return false;

    rec.front_face = !rec.front_face;
    return true;
}

bool FlipFace::bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT {
    return _obj->bounding_box(t0, t1, output_box);
}
