#include "Objects/HittableList.h"
#include "AABB.h"

using namespace std;


HittableList::HittableList(const shared_ptr<IHittable> &object) NOEXCEPT {
    add(object);
}

void HittableList::clear() NOEXCEPT {
    _objects.clear();
}

void HittableList::add(const shared_ptr<IHittable> &object) NOEXCEPT {
    _objects.push_back(object);
}

bool HittableList::hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT {
    HitRecord temp_rec{};
    bool hit_anything = false;
    rreal closest_so_far = t_max;

    // TODO possible to sort objects in some manor to gain speed bump?

    for (const auto & obj : _objects) {
        if (obj->hit(rng, r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::bounding_box(
    const rreal t0,
    const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    if (_objects.empty())
        return false;

    // summate all of the boxes
    AABB temp_box;
    bool first_box = true;

    for (const auto &obj : _objects) {
        // TODO [possible bug], what if a hittable list also contains an empty hittable list?, but in the parent list, there are more elements after the (empty) child list
        if (!obj->bounding_box(t0, t1, temp_box))
            return false;

        output_box = first_box ? temp_box : AABB::surrounding(output_box, temp_box);
        first_box = false;
    }

    return true;
}
