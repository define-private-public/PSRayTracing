#include "Objects/MovingSphere.h"
#include <cmath>
#include "Ray.h"
#include "AABB.h"

using namespace std;

// TODO [refactoring]
// - this shares a lot of common code with the `Sphere`, see if things can be unified
// TODO [bug, feature]
//  - this doesn support textures (UV) like the regular sphere does  look at at the `hit` function of the other


MovingSphere::MovingSphere(
    const Vec3 &center0, const Vec3 &center1,
    const rreal time0, const rreal time1,
    const rreal r,
    const shared_ptr<IMaterial> &mat_ptr
) NOEXCEPT :
    _center0(center0), _center1(center1),
    _time0(time0), _time1(time1),
    _radius(r),
    _mat_ptr(mat_ptr)
{ }

std::shared_ptr<IHittable> MovingSphere::deep_copy() const NOEXCEPT {
    auto ms = make_shared<MovingSphere>(*this);
    ms->_mat_ptr = _mat_ptr->deep_copy();

    return ms;
}

bool MovingSphere::hit(
    [[maybe_unused]] RandomGenerator &rng,
    const Ray &r,
    const rreal t_min,
    const rreal t_max,
    HitRecord &rec
) const NOEXCEPT {
    const Vec3 oc = r.origin - center(r.time);
    const Vec3 ray_dir = r.direction;
    const rreal a = ray_dir.length_squared();
    const rreal half_b = oc.dot(ray_dir);
    const rreal c = oc.length_squared() - (_radius * _radius);
    const rreal discriminant = (half_b * half_b) - (a * c);

#ifdef USE_BOOK_SPHERE_HIT
    if (discriminant > 0) {
        const rreal root = util::sqrt(discriminant);

        rreal temp = (-half_b - root) / a;
        if ((temp < t_max) && (temp > t_min)) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            const Vec3 outward_normal = (rec.p - center(r.time)) / _radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = _mat_ptr;
            return true;
        }

        temp = (-half_b + root) / a;
        if ((temp < t_max) && (temp > t_min)) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            const Vec3 outward_normal = (rec.p - center(r.time)) / _radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = _mat_ptr;
            return true;
        }
    }
#else
    if (discriminant > 0) {
        const rreal root = util::sqrt(discriminant);
        const rreal temp1 = (-half_b - root) / a;
        const rreal temp2 = (-half_b + root) / a;
        const bool hit1 = (temp1 < t_max) && (temp1 > t_min);
        const bool hit2 = (temp2 < t_max) && (temp2 > t_min);

        if (hit1) {
            const Vec3 p1 = r.at(temp1);
            const Vec3 on1 = (p1 - center(r.time)) / _radius;      // on = outward normal
//            sphere_get_uv(on1, rec.u, rec.v);

            rec.t = temp1;
            rec.p = p1;
            rec.set_face_normal(r, on1);
            rec.mat_ptr = _mat_ptr;
            return true;
        }

        if (hit2) {
            const Vec3 p2 = r.at(temp2);
            const Vec3 on2 = (p2 - center(r.time)) / _radius;
 //           sphere_get_uv(on2, rec.u, rec.v);

            rec.t = temp2;
            rec.p = p2;
            rec.set_face_normal(r, on2);
            rec.mat_ptr = _mat_ptr;
            return true;
        }
    }
#endif

    return false;
}

bool MovingSphere::bounding_box(const rreal t0, const rreal t1, AABB &output_box) const NOEXCEPT {
    const Vec3 r(abs(_radius));
    const Vec3 c0 = center(t0);
    const Vec3 c1 = center(t1);

    const AABB box0(c0 - r, c0 + r);
    const AABB box1(c1 - r, c1 + r);

    output_box = AABB::surrounding(box0, box1);
    return true;
}

Vec3 MovingSphere::center(const rreal time) const NOEXCEPT {
    return _center0 + (((time - _time0) / (_time1 - _time0)) * (_center1 - _center0));
}
