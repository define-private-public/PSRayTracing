#pragma once

#include <memory>
#include "Ray.h"
#include "Interfaces/IPDF.h"
#include "PDFVariant.h"


struct ScatterRecord {
    // TODO I think we can reorder these parameters, do a measuring (and compare with books' code on GitHub)
    Ray ray;                                        ///< In books 1 & 2, this argument was called `scattered`, in book 3 is was called `specular_ray`.
    bool is_specular = false;
    Vec3 attenuation;

    #ifdef USE_BOOK_PDF_MANAGEMENT
        std::shared_ptr<IPDF> pdf_ptr = nullptr;
    #else
        PDFVariant pdf;
    #endif
};
