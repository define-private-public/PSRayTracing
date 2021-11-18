#pragma once

#include <memory>
#include "Vec3.h"
#include "Ray.h"
#include "Interfaces/IMaterial.h"


struct HitRecord : public IDeepCopyable<HitRecord> {
    Vec3 p = Vec3();
    Vec3 normal = Vec3();

#ifdef USE_BOOK_MAT_PTR
    // The book by default uses a shared pointer, which is reference counted and slow
    std::shared_ptr<IMaterial> mat_ptr;
#else
    // Instead, we can use a raw pointer, which is more performant
    IMaterial *mat_ptr;
#endif // USE_BOOK_MAT_PTR

    rreal t = 0;     // Time
    rreal u = 0;     // Texture coordinate
    rreal v = 0;     // Texture coordinate
    bool front_face = false;

    inline virtual std::shared_ptr<HitRecord> deep_copy() const NOEXCEPT override {
        auto hr = std::make_shared<HitRecord>(*this);
        hr->set_mat_ptr(mat_ptr->deep_copy());

        return hr;
    }

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) NOEXCEPT {
        front_face = (r.direction.dot(outward_normal) < 0);
        normal = front_face ? outward_normal : -outward_normal;
    }

    inline void set_mat_ptr(const std::shared_ptr<IMaterial> &mp) NOEXCEPT {
    #ifdef USE_BOOK_MAT_PTR
        // Using the book? simply can set it
        mat_ptr = mp;
    #else
        // Else, use the `get()` method
        mat_ptr = mp.get();
    #endif // USE_BOOK_MAT_PTR
    }
};
