#pragma once

#include "Interfaces/IHittable.h"
#include <vector>
#include "AABB.h"
#include "Objects/HittableList.h"


/**
 * This is an alternative implementation of the BVHNode class.  Instead of storing a tree of pointers to
 * other child BVHNodes, it instead constructs a list, that stores paris of `(AABB, index)` objects.  That
 * list is structured in a way that it can be traversed like a BVH tree.
 *
 * Because of the memory layout, organzation, and some other optimzations, it's actually more performant.
 *
 * It's a bit complicated, and I didn't want to clutter it together with the Book's BVHNode code, so I put
 * it in it's own class.
 */
class BVHNode_MorePerformant : public IHittable {
private:
    /*== Class Internal Stuff ==*/

    // This is a small data structure, that ties an AABB, with the index of a hittable object (in a list)
    struct _IndexedAABB {
        size_t object_index;    // Index of the object from `_objects`
        AABB box;               // AABB of the object
    };

    // Node in our logical tree
    // It simply stores boxes, and Ids
    struct _Node {
        AABB box;                           // The surrounding AABB, used for hit detection

        // If we have child nodes, these should be their indices in the list of nodes
        // This also acts as the object to hit if there are no child nodes, (this could be the left side)
        size_t child_a_index = std::numeric_limits<size_t>::max();          // Set these values initially to an impossibly high value
        size_t child_b_index = std::numeric_limits<size_t>::max();

        // If both indices are the same, then we consider that this node's contained indices actually point to an object index
        inline bool contains_object() const NOEXCEPT {
            return (child_a_index == child_b_index);
        }

        // Small helper, which gets the index of the object ot hit, (i.e. draw)
        size_t inline object_index() const NOEXCEPT {
            return child_a_index;
        }

        // Similar helper to above
        inline void set_object_index(const size_t index) NOEXCEPT {
            child_a_index = index;
            child_b_index = index;
        }

        static size_t add_object_to_tree(BVHNode_MorePerformant *bvh_tree, const _IndexedAABB &iaabb);

        static size_t add_empty_node_to_tree(BVHNode_MorePerformant *bvh_tree);
    };

    /*== Box comparison functions.
     *
     * These compare the min point of two boxes, and sees if the first box's is less than the second.
     */
    static inline bool _box_x_compare(const _IndexedAABB &box_a, const _IndexedAABB &box_b) NOEXCEPT {
        return (box_a.box.min.x < box_b.box.min.x);
    }

    static inline bool _box_y_compare(const _IndexedAABB &box_a, const _IndexedAABB &box_b) NOEXCEPT {
        return (box_a.box.min.y < box_b.box.min.y);
    }

    static inline bool _box_z_compare(const _IndexedAABB &box_a, const _IndexedAABB &box_b) NOEXCEPT {
        return (box_a.box.min.z < box_b.box.min.z);
    }
    /*== Class Internal Stuff ==*/



    // Data
    std::vector<std::shared_ptr<IHittable>> _objects;       // All of the objects that are contained in the BVH Tree
    std::vector<_Node> _tree;                               // Our faux tree
    mutable std::vector<size_t> _to_visit;                  // This is used in the `hit()` function as a list of nodes that need to be visited.  It's place here to save on memory allocations/reallocations

public:
    // Constructors
    explicit BVHNode_MorePerformant() = default;
    explicit BVHNode_MorePerformant(RandomGenerator &rng, const HittableList &list, const rreal time0, const rreal time1);

    virtual std::shared_ptr<IHittable> deep_copy() const NOEXCEPT override;

    virtual bool hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT override;
    virtual bool bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT override;

private:
    // Used in the constructor to build the tree
    size_t _build_tree(RandomGenerator &rng, std::vector<_IndexedAABB> boxes, const size_t start, const size_t end);

};
