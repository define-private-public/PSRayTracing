#include "Objects/Box.h"
#include "Objects/Rectangles.h"
#include "BVHNode.h"
#include "RandomGenerator.h"

using namespace std;

// TODO [performance] Might there be a more performant way of dividing up the sides for hit detection
//      possibly exploiting the nature of how BVH nodes are constructed?

Box::Box(
    [[maybe_unused]] RandomGenerator &rng,      // The `rng` will not be used if we're not using the BVH node to organize box internals, this prevents compiler warnings
    const Vec3 &point_min, const Vec3 &point_max, const shared_ptr<IMaterial> &mat) NOEXCEPT :
    _box_min(point_min),
    _box_max(point_max)
{
    _sides.add(make_shared<XYRect>(point_min.x, point_max.x, point_min.y, point_max.y, point_max.z, mat));
    _sides.add(make_shared<XYRect>(point_min.x, point_max.x, point_min.y, point_max.y, point_min.z, mat));

    _sides.add(make_shared<XZRect>(point_min.x, point_max.x, point_min.z, point_max.z, point_max.y, mat));
    _sides.add(make_shared<XZRect>(point_min.x, point_max.x, point_min.z, point_max.z, point_min.y, mat));

    _sides.add(make_shared<YZRect>(point_min.y, point_max.y, point_min.z, point_max.z, point_max.x, mat));
    _sides.add(make_shared<YZRect>(point_min.y, point_max.y, point_min.z, point_max.z, point_min.x, mat));

#ifndef USE_BOOK_BOX_HEIARCHY
    _heiarchy = make_shared<BVHNode>(rng, _sides, 0, 1);
#endif
}

bool Box::hit(RandomGenerator &rng, const Ray &r, const rreal t_min, const rreal t_max, HitRecord &rec) const NOEXCEPT {
#ifdef USE_BOOK_BOX_HEIARCHY
    // Book code uses the `HittableList` for hit detection (slightly slower)
    return _sides.hit(rng, r, t_min, t_max, rec);
#else
    return _heiarchy->hit(rng, r, t_min, t_max, rec);
#endif

    // TODO [peformance] this might be a bit of a bonkers idea, but what if we expanded out the *Rect::hit() code
    //                   here for all 6 sides?  Look at *AABB::hit(), we can duplicate that here, but specify the sides
    //                   explicity (e.g.)  It's very probabale that the compiler will then reorder instructions and stuff
    //                   things into vector instructions for us!
    //
    //                   We might be able to remove BVH node that was added for performance gains
}

bool Box::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = AABB(_box_min, _box_max);
    return true;
}
