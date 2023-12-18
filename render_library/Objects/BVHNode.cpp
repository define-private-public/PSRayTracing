#include "Objects/BVHNode.hpp"
#include <stdexcept>
#include <algorithm>
#include "Objects/HittableList.hpp"
#include "RandomGenerator.hpp"

using namespace std;


BVHNode::BVHNode(RandomGenerator &rng, const HittableList &list, const rreal time0, const rreal time1) :
    BVHNode(rng, list._objects, 0, list._objects.size(), time0, time1)
{ }

BVHNode::BVHNode(
    RandomGenerator &rng,
    vector<shared_ptr<IHittable>> objects,
    const size_t start,
    const size_t end,
    const rreal time0,
    const rreal time1
) {
    // Compare along a random axis
    const int axis = rng.get_int(0, 2);
    auto comparator = box_x_compare;
    if (axis == 1)
        comparator = box_y_compare;
    else if (axis == 2)
        comparator = box_z_compare;

    // Split up objects
    const size_t object_span = end - start;
    if (object_span == 1)
        _left = _right = objects[start];
    else if (object_span == 2) {
        shared_ptr<IHittable> a = objects[start];
        shared_ptr<IHittable> b = objects[start + 1];
        if (comparator(a, b)) {
            _left = a;
            _right = b;
        } else {
            _left = b;
            _right = a;
        }
    } else {
        sort(
            next(objects.begin(), static_cast<int>(start)),
            next(objects.begin(), static_cast<int>(end)),
            comparator
        );

        const size_t mid = start + (object_span / 2);
        _left = make_shared<BVHNode>(rng, objects, start, mid, time0, time1);
        _right = make_shared<BVHNode>(rng, objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    const bool not_box_left = !_left->bounding_box(time0, time1, box_left);
    const bool not_box_right = !_right->bounding_box(time0, time1, box_right);
    if (not_box_left || not_box_right)
        throw runtime_error("No bounding box in BVHNode constructor");

    _box = AABB::surrounding(box_left, box_right);
}

shared_ptr<IHittable> BVHNode::deep_copy() const NOEXCEPT {
    auto node = make_shared<BVHNode>(*this);
    node->_left = _left->deep_copy();
    node->_right = _right->deep_copy();

    return node;
}

bool BVHNode::hit(
    RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
    // If it didn't hit our box, then it didn't hit any children
    if (!_box.hit(r, t_min, t_max))
        return false;

    const bool hit_left = _left->hit(rng, r, t_min, t_max, rec);
    const bool hit_right = _right->hit(rng, r, t_min, (hit_left ? rec.t : t_max), rec);

    return (hit_left || hit_right);
}

bool BVHNode::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = _box;
    return true;
}

bool box_compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b, const Vec3::Axis &axis) {
    AABB box_a, box_b;

    // TODO what about time parameters?
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        throw runtime_error("No bounding box in BVHNode constructor [box_compare]");

    // original implementation used indexing here
    // but that is because of how the Vec3 class was structured
    switch (axis) {
        case Vec3::Axis::X: return (box_a.min.x < box_b.min.x);
        case Vec3::Axis::Y: return (box_a.min.y < box_b.min.y);
        case Vec3::Axis::Z: return (box_a.min.z < box_b.min.z);
    }

    // Should never hit this case
    throw runtime_error("Proper Vec3::Axis value was never provided");
}
