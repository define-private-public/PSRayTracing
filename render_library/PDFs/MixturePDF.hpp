#pragma once

#include "Interfaces/IPDF.hpp"
#include <memory>


// A PDF that mixes two others
class MixturePDF FINAL : public IPDF {
private:
    // Data
    #ifdef USE_BOOK_PDF_MANAGEMENT
        std::shared_ptr<IPDF> _a;
        std::shared_ptr<IPDF> _b;
    #else
        // Shared pointers are slow, so on our PDFVariant change, we use raw pointers instead
        const IPDF *_a = nullptr;
        const IPDF *_b = nullptr;
    #endif

public:
    explicit MixturePDF(
        // (Putting pre-preprocessor macro in the arguments...)
        //    (Yes, this is disgusting, and I am sorry...)
        #ifdef USE_BOOK_PDF_MANAGEMENT
            const std::shared_ptr<IPDF> &a,
            const std::shared_ptr<IPDF> &b
        #else
            const IPDF *a,
            const IPDF *b
        #endif
    ) NOEXCEPT :
        _a(a),
        _b(b)
    { }

    rreal value(RandomGenerator &rng, const Vec3 &direction) const NOEXCEPT override;
    Vec3 generate(RandomGenerator &rng) const NOEXCEPT override;
};
