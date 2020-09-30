#pragma once

#include <memory>
#include "Vec3.h"
#include "Ray.h"
class IMaterial;


struct HitRecord {
    Vec3 p = Vec3();
    Vec3 normal = Vec3();
    std::shared_ptr<IMaterial> mat_ptr;
    rreal t = 0;     // Time
    rreal u = 0;     // Texture coordinate
    rreal v = 0;     // Texture coordinate
    bool front_face = false;

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) NOEXCEPT {
        front_face = (r.direction.dot(outward_normal) < 0);
        normal = front_face ? outward_normal : -outward_normal;
    }
};
