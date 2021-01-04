#pragma once

#include <memory>
#include "Ray.h"
#include "Interfaces/IPDF.h"


struct ScatterRecord {
    Ray specular_ray;
    bool is_specular = false;
    Vec3 attenuation;
    std::shared_ptr<IPDF> pdf_ptr = nullptr;
};
