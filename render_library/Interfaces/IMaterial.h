#pragma once

#include "Common.h"
#include "Vec3.h"
#include "Ray.h"
#include "RenderMethod.h"
#include "Interfaces/IDeepCopyable.h"

struct HitRecord;
struct ScatterRecord;
class RandomGenerator;
class IPDF;


class IMaterial : public IDeepCopyable<IMaterial> {
public:
    virtual ~IMaterial() NOEXCEPT = default;

    virtual std::shared_ptr<IMaterial> deep_copy() const NOEXCEPT = 0;

    virtual bool scatter(
        RandomGenerator &rng,       // In
        const Ray &r_in,            // In
        const HitRecord &h_rec,     // In
        ScatterRecord &s_rec,       // Out
        const RenderMethod method   // In; this argument doesn't exist in any of the books, but it's here so we can support rendering all three books at the same time
    ) const NOEXCEPT = 0;

    virtual rreal scattering_pdf(
        const Ray &r_in,
        const HitRecord &h_rec,
        const Ray &scattered
    ) const NOEXCEPT = 0;

    virtual Vec3 emitted(
        const Ray &r_in,
        const HitRecord &h_rec,
        const rreal u,
        const rreal v,
        const Vec3 &p,
        const RenderMethod method   // this argument doesn't exist in any of the books, but it's here so we can support rendering all three books at the same time
    ) const NOEXCEPT = 0;
};
