#pragma once

#include <memory>
#include "Ray.h"
#include "Interfaces/IPDF.h"
#include "PDFVariant.h"


struct ScatterRecord {
    Ray specular_ray;
    bool is_specular = false;
    Vec3 attenuation;
    PDFVariant pdf;
};
