#include "Objects/Rectangles.h"

using namespace std;


// the bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount
constexpr auto ThinPadding = static_cast<rreal>(0.0001);


XYRect::XYRect(
    const rreal x0,
    const rreal x1,
    const rreal y0,
    const rreal y1,
    const rreal k,
    shared_ptr<IMaterial> mat
) NOEXCEPT :
    _x0(x0),
    _x1(x1),
    _y0(y0),
    _y1(y1),
    _k(k),
    _mat_ptr(mat)
{ }

shared_ptr<IHittable> XYRect::deep_copy() const NOEXCEPT {
    auto r = make_shared<XYRect>(*this);
    r->_mat_ptr = _mat_ptr->deep_copy();

    return r;
}

bool XYRect::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
#ifdef USE_BOOK_RECT_HIT
    const rreal t = (_k - r.origin.z) / r.direction.z;
    if ((t < t_min) || (t > t_max))
        return false;

    const rreal x = r.origin.x + (t * r.direction.x);
    const rreal y = r.origin.y + (t * r.direction.y);
    if ((x < _x0) || (x > _x1) || (y < _y0) || (y > _y1))
        return false;

    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (y - _y0) / (_y1 - _y0);
    rec.t = t;
    rec.p = r.at(t);
#else
    // This is slighlty faster than the book's code because we're avoiding an extra branch check, and the compiler
    // is moving things around to compute them with vector instructions
    const rreal t = (_k - r.origin.z) / r.direction.z;
    const rreal x = r.origin.x + (t * r.direction.x);
    const rreal y = r.origin.y + (t * r.direction.y);

    // TODO [performance] would it be faster to compute this result after the branch?
    const rreal u = (x - _x0) / (_x1 - _x0);
    const rreal v = (y - _y0) / (_y1 - _y0);
    const Vec3 p = r.at(t);

    if (((t < t_min) || (t > t_max)) || ((x < _x0) || (x > _x1) || (y < _y0) || (y > _y1)))
        return false;

    rec.u = u;
    rec.v = v;
    rec.t = t;
    rec.p = p;
#endif

    rec.set_face_normal(r, Vec3(0, 0, 1));
    rec.set_mat_ptr(_mat_ptr);

    return true;
}

bool XYRect::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = AABB(
        Vec3(_x0, _y0, _k - ThinPadding),
        Vec3(_x1, _y1, _k + ThinPadding)
    );
    return true;
}





XZRect::XZRect(
    const rreal x0,
    const rreal x1,
    const rreal z0,
    const rreal z1,
    const rreal k,
    shared_ptr<IMaterial> mat
) NOEXCEPT :
    _x0(x0),
    _x1(x1),
    _z0(z0),
    _z1(z1),
    _k(k),
    _mat_ptr(mat)
{ }

shared_ptr<IHittable> XZRect::deep_copy() const NOEXCEPT {
    auto r = make_shared<XZRect>(*this);
    r->_mat_ptr = _mat_ptr->deep_copy();

    return r;
}

bool XZRect::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
#ifdef USE_BOOK_RECT_HIT
    const rreal t = (_k - r.origin.y) / r.direction.y;
    if ((t < t_min) || (t > t_max))
        return false;

    const rreal x = r.origin.x + (t * r.direction.x);
    const rreal z = r.origin.z + (t * r.direction.z);
    if ((x < _x0) || (x > _x1) || (z < _z0) || (z > _z1))
        return false;

    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.t = t;
    rec.p = r.at(t);
#else
    const rreal t = (_k - r.origin.y) / r.direction.y;
    const rreal x = r.origin.x + (t * r.direction.x);
    const rreal z = r.origin.z + (t * r.direction.z);

    // TODO [performance] would it be faster to compute this result after the branch?
    const rreal u = (x - _x0) / (_x1 - _x0);
    const rreal v = (z - _z0) / (_z1 - _z0);
    const Vec3 p = r.at(t);

    if (((t < t_min) || (t > t_max)) || ((x < _x0) || (x > _x1) || (z < _z0) || (z > _z1)))
        return false;

    rec.u = u;
    rec.v = v;
    rec.t = t;
    rec.p = p;
#endif

    rec.set_face_normal(r, Vec3(0, 1, 0));
    rec.set_mat_ptr(_mat_ptr);

    return true;
}

bool XZRect::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = AABB(
        Vec3(_x0, _k - ThinPadding, _z0),
        Vec3(_x1, _k + ThinPadding, _z1)
    );
    return true;
}





YZRect::YZRect(
    const rreal y0,
    const rreal y1,
    const rreal z0,
    const rreal z1,
    const rreal k,
    shared_ptr<IMaterial> mat
) NOEXCEPT :
    _y0(y0),
    _y1(y1),
    _z0(z0),
    _z1(z1),
    _k(k),
    _mat_ptr(mat)
{ }

shared_ptr<IHittable> YZRect::deep_copy() const NOEXCEPT {
    auto r = make_shared<YZRect>(*this);
    r->_mat_ptr = _mat_ptr->deep_copy();

    return r;
}

bool YZRect::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
#ifdef USE_BOOK_RECT_HIT
    const rreal t = (_k - r.origin.x) / r.direction.x;
    if ((t < t_min) || (t > t_max))
        return false;

    const rreal y = r.origin.y + (t * r.direction.y);
    const rreal z = r.origin.z + (t * r.direction.z);
    if ((y < _y0) || (y > _y1) || (z < _z0) || (z > _z1))
        return false;

    rec.u = (y - _y0) / (_y1 - _y0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.t = t;
    rec.p = r.at(t);
#else
    const rreal t = (_k - r.origin.x) / r.direction.x;
    const rreal y = r.origin.y + (t * r.direction.y);
    const rreal z = r.origin.z + (t * r.direction.z);

    // TODO [performance] would it be faster to compute this result after the branch?
    const rreal u = (y - _y0) / (_y1 - _y0);
    const rreal v = (z - _z0) / (_z1 - _z0);
    const Vec3 p = r.at(t);

    if (((t < t_min) || (t > t_max)) || ((y < _y0) || (y > _y1) || (z < _z0) || (z > _z1)))
        return false;

    rec.u = u;
    rec.v = v;
    rec.t = t;
    rec.p = p;
#endif

    rec.set_face_normal(r, Vec3(1, 0, 0));
    rec.set_mat_ptr(_mat_ptr);

    return true;
}

bool YZRect::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = AABB(
        Vec3(_k - ThinPadding, _y0, _z0),
        Vec3(_k + ThinPadding, _y1, _z1)
    );
    return true;
}
