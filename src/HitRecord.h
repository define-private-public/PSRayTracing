#pragma once

#include <memory>
#include "Vec3.h"
#include "Ray.h"
#include "Interfaces/IMaterial.h"


struct HitRecord : public IDeepCopyable<HitRecord> {
    Vec3 p = Vec3();
    Vec3 normal = Vec3();
    std::shared_ptr<IMaterial> mat_ptr;
    rreal t = 0;     // Time
    rreal u = 0;     // Texture coordinate
    rreal v = 0;     // Texture coordinate
    bool front_face = false;

    inline virtual std::shared_ptr<HitRecord> deep_copy() const NOEXCEPT override {
        auto hr = std::make_shared<HitRecord>(*this);
        hr->mat_ptr = mat_ptr->deep_copy();

        return hr;
    }

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) NOEXCEPT {
        front_face = (r.direction.dot(outward_normal) < 0);
        normal = front_face ? outward_normal : -outward_normal;
    }
};
