#include "Objects/Box.h"
#include "Objects/Rectangles.h"
#include "RandomGenerator.h"

using namespace std;

// These are indicies that are used in my method for Box::hit()
constexpr size_t Back = 0;
constexpr size_t Front = 1;
constexpr size_t Top = 2;
constexpr size_t Bottom = 3;
constexpr size_t Left = 4;
constexpr size_t Right = 5;


Box::Box(const Vec3 &point_min, const Vec3 &point_max, const shared_ptr<IMaterial> &mat) NOEXCEPT :
    _box_min(point_min),
    _box_max(point_max)
{
#ifdef USE_BOOK_BOX
    // Using the book's Box, we need to add all of the sides as children
    _sides.add(make_shared<XYRect>(point_min.x, point_max.x, point_min.y, point_max.y, point_max.z, mat));
    _sides.add(make_shared<XYRect>(point_min.x, point_max.x, point_min.y, point_max.y, point_min.z, mat));

    _sides.add(make_shared<XZRect>(point_min.x, point_max.x, point_min.z, point_max.z, point_max.y, mat));
    _sides.add(make_shared<XZRect>(point_min.x, point_max.x, point_min.z, point_max.z, point_min.y, mat));

    _sides.add(make_shared<YZRect>(point_min.y, point_max.y, point_min.z, point_max.z, point_max.x, mat));
    _sides.add(make_shared<YZRect>(point_min.y, point_max.y, point_min.z, point_max.z, point_min.x, mat));
#else
    // This should really be in the initializer list, but I don't want to add extra #ifdefs...
    _mat = mat;
    bounding_box(0, 1, _aabb);
#endif  // USE_BOOK_BOX
}

bool Box::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
#ifdef USE_BOOK_BOX
    // Book code uses the `HittableList` for hit detection (slower)
    return _sides.hit(rng, r, t_min, t_max, rec);
#else
    // This is faster than the book's code because:
    // - We're using stack memory (instead of dynamic)
    //   - And less memory
    // - Less (possible) pointer chasing
    // - More cache friendly
    // - SIMD/Vectorized instructions
    if (!_aabb.hit(r, t_min, t_max))
        return false;

    // The `k` values that are used for `Rect` construction
    const rreal k[6] {
        _box_max.z,         // Back
        _box_min.z,         // Front
        _box_max.y,         // Top
        _box_min.y,         // Bottom
        _box_max.x,         // Left
        _box_min.x          // Right
    };

    const rreal t[6] {
        (k[Back]   - r.origin.z) / r.direction.z,
        (k[Front]  - r.origin.z) / r.direction.z,
        (k[Top]    - r.origin.y) / r.direction.y,
        (k[Bottom] - r.origin.y) / r.direction.y,
        (k[Left]   - r.origin.x) / r.direction.x,
        (k[Right]  - r.origin.x) / r.direction.x
    };

    const rreal x[6] {
        r.origin.x + (t[Back]   * r.direction.x),
        r.origin.x + (t[Front]  * r.direction.x),
        r.origin.x + (t[Top]    * r.direction.x),
        r.origin.x + (t[Bottom] * r.direction.x),
        0,
        0
    };

    const rreal y[6] {
        r.origin.y + (t[Back]  * r.direction.y),
        r.origin.y + (t[Front] * r.direction.y),
        0,
        0,
        r.origin.y + (t[Left]  * r.direction.y),
        r.origin.y + (t[Right] * r.direction.y)
    };

    const rreal z[6] {
        0,
        0,
        r.origin.z + (t[Top]    * r.direction.z),
        r.origin.z + (t[Bottom] * r.direction.z),
        r.origin.z + (t[Left]   * r.direction.z),
        r.origin.z + (t[Right]  * r.direction.z)
    };

    const bool did_hit[6] {
        (t[Back]   > t_min) && (t[Back]   < t_max) && (x[Back]   > _box_min.x) && (x[Back]   < _box_max.x) && (y[Back]   > _box_min.y) && (y[Back]   < _box_max.y),
        (t[Front]  > t_min) && (t[Front]  < t_max) && (x[Front]  > _box_min.x) && (x[Front]  < _box_max.x) && (y[Front]  > _box_min.y) && (y[Front]  < _box_max.y),
        (t[Top]    > t_min) && (t[Top]    < t_max) && (x[Top]    > _box_min.x) && (x[Top]    < _box_max.x) && (z[Top]    > _box_min.z) && (z[Top]    < _box_max.z),
        (t[Bottom] > t_min) && (t[Bottom] < t_max) && (x[Bottom] > _box_min.x) && (x[Bottom] < _box_max.x) && (z[Bottom] > _box_min.z) && (z[Bottom] < _box_max.z),
        (t[Left]   > t_min) && (t[Left]   < t_max) && (y[Left]   > _box_min.y) && (y[Left]   < _box_max.y) && (z[Left]   > _box_min.z) && (z[Left]   < _box_max.z),
        (t[Right]  > t_min) && (t[Right]  < t_max) && (y[Right]  > _box_min.y) && (y[Right]  < _box_max.y) && (z[Right]  > _box_min.z) && (z[Right]  < _box_max.z),
    };

    // First see if any of them hit
    const bool any_hit = did_hit[Back] || did_hit[Front] || did_hit[Top] || did_hit[Bottom] || did_hit[Left] || did_hit[Right];
    if (!any_hit)
        return false;

    // Create an array,  If that side was hit, use it's `t` if not, set that to Infinity.
    //   then we use `std::min()` to find the miniumum `t` (i.e. closest) value.
    using IndexedHit = pair<rreal, size_t>;
    const IndexedHit nearest = std::min({
            make_pair((did_hit[Back]   ? t[Back]   : Infinity), Back),
            make_pair((did_hit[Front]  ? t[Front]  : Infinity), Front),
            make_pair((did_hit[Top]    ? t[Top]    : Infinity), Top),
            make_pair((did_hit[Bottom] ? t[Bottom] : Infinity), Bottom),
            make_pair((did_hit[Left]   ? t[Left]   : Infinity), Left),
            make_pair((did_hit[Right]  ? t[Right]  : Infinity), Right)
        },
        [] (const IndexedHit &a, const IndexedHit &b) { return a.first < b.first; }
    );

    const rreal nearest_t = nearest.first;
    const size_t nearest_i = nearest.second;

    // Texturing coordinates
    const rreal u[6] {
        (x[Back]   - _box_min.x) / (_box_max.x - _box_min.x),
        (x[Front]  - _box_min.x) / (_box_max.x - _box_min.x),
        (x[Top]    - _box_min.x) / (_box_max.x - _box_min.x),
        (x[Bottom] - _box_min.x) / (_box_max.x - _box_min.x),
        (y[Left]   - _box_min.y) / (_box_max.y - _box_min.y),
        (y[Right]  - _box_min.y) / (_box_max.y - _box_min.y)
    };

    const rreal v[6] {
        (z[Back]   - _box_min.y) / (_box_max.y - _box_min.y),
        (z[Front]  - _box_min.y) / (_box_max.y - _box_min.y),
        (z[Top]    - _box_min.z) / (_box_max.z - _box_min.z),
        (z[Bottom] - _box_min.z) / (_box_max.z - _box_min.z),
        (z[Left]   - _box_min.z) / (_box_max.z - _box_min.z),
        (z[Right]  - _box_min.z) / (_box_max.z - _box_min.z)
    };

    constexpr Vec3 face_normals[6] {
        Vec3(0, 0, 1), Vec3(0, 0, 1),           // Back & Front
        Vec3(0, 1, 0), Vec3(0, 1, 0),           // Top & Bottom
        Vec3(1, 0, 0), Vec3(1, 0, 0),           // Left & Right
    };

    // Setup the rest of the hit record
    rec.u = u[nearest_i];
    rec.v = v[nearest_i];
    rec.t = nearest_t;
    rec.p = r.at(nearest_t);
    rec.set_face_normal(r, face_normals[nearest_i]);
    rec.mat_ptr = _mat;

    return true;
#endif // USE_BOOK_BOX (else clause)
}

bool Box::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = AABB(_box_min, _box_max);
    return true;
}
