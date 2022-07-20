#include "Objects/BVHNode_MorePerformant.h"
#include <stdexcept>
#include <algorithm>
#include <utility>
#include "Objects/HittableList.h"
#include "RandomGenerator.h"

using namespace std;


/*== _Node methods ==*/

/**
 * Add a node, that contains a hittable object, to the BVH Tree
 */
size_t BVHNode_MorePerformant::_Node::add_object_to_tree(BVHNode_MorePerformant *bvh_tree, const _IndexedAABB &iaabb) {
    // We have a single object that needs to be put into a node
    _Node node;
    node.box = iaabb.box;
    node.set_object_index(iaabb.object_index);

    // Put it the node into the virtual tree
    bvh_tree->_tree.emplace_back(node);

    // Since we guarentee that the object is placed at the back, get it's insertion index by looking at the (size - 1)
    return (bvh_tree->_tree.size() - 1);
}

/**
 * Add a node, that contains nothing, to the BVH Tree.
 */
size_t BVHNode_MorePerformant::_Node::add_empty_node_to_tree(BVHNode_MorePerformant *bvh_tree) {
    // This node will be used to have children, so it's a "box node"
    _Node node;

    // Put the node into the virtula tree
    bvh_tree->_tree.emplace_back(node);

    // Since we guartentee that the object is placed at the back, get it's insersion index (by looking at the (size - 1))
    return (bvh_tree->_tree.size() - 1);
}



BVHNode_MorePerformant::BVHNode_MorePerformant(RandomGenerator &rng, const HittableList &list, const rreal time0, const rreal time1) :
    _objects(list._objects)
{
    const size_t num_objs = _objects.size();

    // First figure out all of the AABB for the objects
    vector<_IndexedAABB> all_boxes;
    all_boxes.reserve(num_objs);

    for (size_t i = 0; i < num_objs; i++) {
        // Create the AABB (with object index)
        _IndexedAABB iaabb;
        iaabb.object_index = i;
        _objects[i]->bounding_box(time0, time1, iaabb.box);

        // Plop it in
        all_boxes.emplace_back(iaabb);
    }

    // Pre-alloc tree storage; With checking a few runs, this is what the tree size comes out to be
    _tree.reserve((2 * num_objs) - 1);

    // Build the tree
    _build_tree(rng, all_boxes, 0, all_boxes.size());

    // Pre-alloc the space for our visiting list, that used in the hit() method
    _to_visit.reserve(_tree.size());
}

shared_ptr<IHittable> BVHNode_MorePerformant::deep_copy() const NOEXCEPT {
    // First use the copy constructor to copy some things automatically for us (e.g. the built BVH tree-list)
    auto node = make_shared<BVHNode_MorePerformant>(*this);

    // Need to clear out the list and then do a deep copy on all objects
    node->_objects.clear();
    for (const auto &obj : _objects)
        node->_objects.emplace_back(obj->deep_copy());

    return node;
}

bool BVHNode_MorePerformant::hit(
    RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
    bool hit_object = false;
    rreal cur_t_max = t_max;

    // Clear out our hit visiting list (memory still should be allocated though)
    _to_visit.clear();
    _to_visit.emplace_back(0);      // Pop on the root of the tree to first visit it.

    // Traverse the "tree-list"
    while (_to_visit.size() > 0) {
        // Pop of the last most
        const size_t n = _to_visit.back();
        _to_visit.pop_back();
        const _Node *node = &_tree[n];

        if (node->box.hit(r, t_min, cur_t_max)) {
            // Does the node contain an object?
            if (node->contains_object()) {
                // Found an object to do a hit test
                hit_object |= _objects[node->object_index()]->hit(
                    rng,
                    r,
                    t_min,
                    cur_t_max,
                    rec
                );

                if (hit_object)
                    cur_t_max = rec.t;
            } else {
                // Need to check the children next, pop on their indices
                _to_visit.emplace_back(node->child_a_index);
                _to_visit.emplace_back(node->child_b_index);
            }
        }
    }

    return hit_object;
}

bool BVHNode_MorePerformant::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = _tree[0].box;
    return true;
}

/**
 * Builds the "tree-list" in the BVHNode, in a recursive manor; this should only be called in the constructor.
 *
 * It's very similar to the constructor for the Book's BVH node.  The main difference is that we are not working with
 * poitners, but indicies instead.  It still tries to construct the "tree-list" in a DFS manor, as the original BVH node
 * does.
 *
 * Its return value is the index, in the tree list, where a box was added.
 *
 * If this confuses you, I'd suggest doing a run through of this code with about 4 or 5 `boxes`.
 */
size_t BVHNode_MorePerformant::_build_tree(RandomGenerator &rng, vector<_IndexedAABB> boxes, const size_t start, const size_t end) {
    // Compare along a random axis
    const int compare_axis = rng.get_int(0, 2);
    auto comparator_func =  (compare_axis == 0) ? _box_x_compare :
                           ((compare_axis == 1) ? _box_y_compare : _box_z_compare);

    const size_t object_span = end - start;

    // One object? Put the box (and the index) into the tree-list
    if (object_span == 1)
        return _Node::add_object_to_tree(this, boxes[start]);

    // Only two? Put make two object notes and stuff it into the tree-list
    if (object_span == 2) {
        // The Boxes
        const _IndexedAABB box_a = boxes[start];
        const _IndexedAABB box_b = boxes[start + 1];

        // Two objects that need to be in nodes
        const size_t parent_index = _Node::add_empty_node_to_tree(this);
        const size_t child_a_index = _Node::add_object_to_tree(this, box_a);
        const size_t child_b_index = _Node::add_object_to_tree(this, box_b);

        // I'm having some issue with the `auto` keywork for the lambda
        bool less = comparator_func(box_a, box_b);

        // Add the surrounding box, and assign the children
        _tree[parent_index].box = AABB::surrounding(box_a.box, box_b.box);
        _tree[parent_index].child_a_index = (less ? child_a_index : child_b_index);
        _tree[parent_index].child_b_index = (less ? child_b_index : child_a_index);

        return parent_index;
    }

    /*== Else, we must have 3+ objects, need to make children ==*/

    // Sort the boxes (based on a comparison axis)
    sort(
        next(boxes.begin(), static_cast<int>(start)),
        next(boxes.begin(), static_cast<int>(end)),
        comparator_func
    );

    const size_t mid = start + (object_span / 2);

    // Add an empty (parent) node to the tree
    const size_t parent_index = _Node::add_empty_node_to_tree(this);
    const size_t first_id = _build_tree(rng, boxes, start, mid);
    const size_t second_id = _build_tree(rng, boxes, mid,   end);

    // Set the data for the parent
    _tree[parent_index].box = AABB::surrounding(_tree[first_id].box, _tree[second_id].box);
    _tree[parent_index].child_a_index =  first_id;
    _tree[parent_index].child_b_index = second_id;

    return parent_index;
}
