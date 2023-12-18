#include "Objects/HittableList.hpp"
#include "AABB.hpp"
#include "RandomGenerator.hpp"

using namespace std;


HittableList::HittableList(const shared_ptr<IHittable> &object) NOEXCEPT {
    add(object);
}

shared_ptr<IHittable> HittableList::deep_copy() const NOEXCEPT {
    auto hl = make_shared<HittableList>(*this);

    // Need to clear out the list and then do a deep copy on all objects
    hl->clear();
    for (const auto &obj : _objects)
        hl->add(obj->deep_copy());

    return hl;
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

rreal HittableList::pdf_value(RandomGenerator &rng, const Vec3 &origin, const Vec3 &v) const NOEXCEPT {
    // `rreal` is an alias for `double`
    const rreal weight = 1.0 / static_cast<rreal>(_objects.size());
    rreal sum = 0;

    for (const shared_ptr<IHittable> &obj : _objects)
        sum += (weight * obj->pdf_value(rng, origin, v));

    return sum;
}

Vec3 HittableList::random(RandomGenerator &rng, const Vec3 &origin) const NOEXCEPT {
    const auto int_size = static_cast<int>(_objects.size());
    const auto index = static_cast<size_t>(rng.get_int(0, int_size - 1));
    return _objects[index]->random(rng, origin);
}
