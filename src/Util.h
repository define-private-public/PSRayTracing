#pragma once

#include "Common.h"
#include <string>
#include <algorithm>
#include <cmath>

// TODO [misc]
// - there should be a SIGGRAPH paper that presented an alternative to the common slick approxmation

// A small set of utility functions
namespace util {
    inline rreal degrees_to_radians(const rreal degrees) NOEXCEPT {
        return degrees * Pi / static_cast<rreal>(180);
    }

    inline rreal clamp(const rreal x, const rreal lower, const rreal upper) NOEXCEPT {
        #ifdef USE_BOOK_CLAMP
            if (x < lower)
                return lower;
            if (x > upper)
                return upper;

            return x;
        #else
            // C++17 and above has a built-in clamp function.   if not, it can also be recreated with `min()` and `max()` calls
            // After inspecting on godbolt, using std::clamp save about one instruction.  But after measuring
            // the effects seemed almost negligable.
            return std::clamp(x, lower, upper);
        #endif
    }

    // Note, this is actually an approximation by Christophine Schlick for determine when a material
    // becomes mirrorlike at an angle
    inline rreal schlick(const rreal cosine, const rreal refractive_index) NOEXCEPT {
        #ifdef USE_BOOK_SCHLICK
            rreal r0 = (1 - refractive_index) / (1 + refractive_index);
            r0 = r0 * r0;
            return r0 + ((1 - r0) * std::pow((1 - cosine), 5));
        #else
            // This tries to take advantage of the compiler converting these math operations
            // into vectorized instructions.  std::pow() can also be kinda slow compared to
            // writing out all of the multiplies.
            //    https://baptiste-wicht.com/posts/2017/09/cpp11-performance-tip-when-to-use-std-pow.html
            const rreal alpha = 1 - cosine;
            const rreal beta = alpha * alpha * alpha * alpha * alpha;

            rreal r0 = (1 - refractive_index) / (1 + refractive_index);
            r0 = r0 * r0;

            return r0 + ((1 - r0) * beta);
        #endif
    }

    #ifndef USE_BOOK_SIN_COS
        rreal _cos_approx_private(rreal x) NOEXCEPT;
    #endif

    inline rreal cos(const rreal x) NOEXCEPT {
        #ifdef USE_BOOK_SIN_COS
            return std::cos(x);
        #else
            return _cos_approx_private(x);
        #endif
    }

    inline rreal sin(const rreal x) NOEXCEPT {
        #ifdef USE_BOOK_SIN_COS
            return std::sin(x);
        #else
            // Due to trig identities, we can reuse our cosine computation (with an offset)
            return util::cos(HalfPi - x);
        #endif
    }


    #ifndef USE_BOOK_ATAN2
        rreal _atan2_approx_private(const rreal y, const rreal x) NOEXCEPT;
    #endif

    inline rreal atan2(const rreal y, const rreal x) NOEXCEPT {
        #ifdef USE_BOOK_ATAN2
            return std::atan2(y, x);
        #else
            return _atan2_approx_private(y, x);
        #endif
    }



    #ifndef USE_BOOK_ASIN
        rreal _asin_approx_private(const rreal x) NOEXCEPT;
    #endif

    inline rreal asin(const rreal x) NOEXCEPT {
        #ifdef USE_BOOK_ASIN
            return std::asin(x);
        #else
            return _asin_approx_private(x);
        #endif
    }


    #ifndef USE_BOOK_SQRT
        rreal _sqrt_approx_private(const rreal x) NOEXCEPT;
    #endif

    inline rreal sqrt(const rreal x) NOEXCEPT {
        #ifdef USE_BOOK_SQRT
            return std::sqrt(x);
        #else
            return _sqrt_approx_private(x);
        #endif
    }


    bool try_parse_size_string(const std::string &s, uint16_t &x, uint16_t &y);
}
