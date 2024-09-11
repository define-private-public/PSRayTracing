#include "Objects/Quad.hpp"
#include "Ray.hpp"
#include "RandomGenerator.hpp"
using namespace std;


Quad::Quad(const Vec3 &origin, const Vec3 &u, const Vec3 &v, const shared_ptr<IMaterial> &material) NOEXCEPT :
    _Q(origin),
    _u(u),
    _v(v),
    _mat_ptr(material)
{
    const Vec3 n = _u.cross(_v);
    _area = n.length();
    _normal = n.unit_vector();
    _D = _normal.dot(_Q);
    _w = n / n.dot(n);

    _set_bounding_box();
}


shared_ptr<IHittable> Quad::deep_copy() const NOEXCEPT
{
    auto r = make_shared<Quad>(*this);
    r->_mat_ptr = _mat_ptr->deep_copy();

    return r;
}


bool Quad::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
    const rreal denom = _normal.dot(r.direction);

    // No ray hit if it's parallel to the plane
    if (std::fabs(denom) < 1e-8)
    {
        return false;
    }

    // Return false if the hit point parameter t is outside the interval
    const rreal t = (_D - _normal.dot(r.origin)) / denom;
    const bool t_in_range = (t_min <= t) && (t <= t_max);
    if (!t_in_range)
    {
        return false;
    }

    // Determine if the hit point lies within the planar shape using its plane coordinates
    const Vec3 intersection = r.at(t);
    const Vec3 planar_hitpt_vector = intersection - _Q;
    const Vec3 p_cross_v = planar_hitpt_vector.cross(_v);
    const Vec3 u_cross_p = _u.cross(planar_hitpt_vector);
    const rreal alpha = _w.dot(p_cross_v);
    const rreal beta = _w.dot(u_cross_p);

    if (!_is_interior(alpha, beta, rec))
    {
        return false;
    }

    // Ray hits a 2D shape
    rec.t = t;
    rec.p = intersection;
    rec.set_mat_ptr(_mat_ptr);
    rec.set_face_normal(r, _normal);

    return true;
}


bool Quad::bounding_box(
    [[maybe_unused]] const rreal t0,
    [[maybe_unused]] const rreal t1,
    AABB &output_box
) const NOEXCEPT {
    output_box = _bounding_box;
    return true;
}


rreal Quad::pdf_value(RandomGenerator &rng, const Vec3 &origin, const Vec3 &v) const NOEXCEPT
{
    HitRecord rec;
    const bool did_hit = hit(rng, Ray(origin, v), 0.001, Infinity, rec);
    if (!did_hit)
    {
        return 0;
    }

    const rreal distance_squared = rec.t * rec.t * v.length_squared();
    const rreal cosine = std::fabs(v.dot(rec.normal)) / v.length();

    return distance_squared / (cosine * _area);
}


Vec3 Quad::random(RandomGenerator &rng, const Vec3 &origin) const NOEXCEPT
{
    const Vec3 p = _Q + (rng.get_real() * _u) + (rng.get_real() * _v);
    return p - origin;
}


void Quad::_set_bounding_box()
{
    // Compute the bounding box of all four vertices
    const auto diagonal_1 = AABB(_Q, (_Q + _u + _v));
    const auto diagonal_2 = AABB((_Q + _u), (_Q + _v));
    _bounding_box = AABB::surrounding(diagonal_1, diagonal_2);
}


bool Quad::_is_interior(const rreal a, const rreal b, HitRecord &rec) const
{
    // This project doesn't have the new `interval` object that's present in v4.x.x of the book,
    //   so we manually place the logic in here
    //
    //

    // The Book's code is doing a double negative/inverse case check.  That's not a good idea IMO...
//    const bool a_not_in_range = !((0.0 <= a) && (a <= 1.0));
//    const bool b_not_in_range = !((0.0 <= b) && (b <= 1.0));
//
//    if (a_not_in_range || b_not_in_range)
//    {
//        return false;
//    }
//    else
//    {
//        rec.u = a;
//        rec.v = b;
//        return true;
//    }

    // For the plane, this check if we hit the quad and record its UV coordiantes
    const bool a_in_range = ((0.0 <= a) && (a <= 1.0));
    const bool b_in_range = ((0.0 <= b) && (b <= 1.0));
    const bool in_range = (a_in_range && b_in_range);

    if (in_range)
    {
        rec.u = a;
        rec.v = b;
    }

    return in_range;
}
