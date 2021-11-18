#include "Util.h"
#include <algorithm>

using namespace std;

namespace util {


#ifndef USE_BOOK_ATAN2
/*!
 * We have our own implementation of `atan2` that can be used that's faster than the standard
 * cmath one, but it does produce slightly different results (an approximation).
 *
 * But see the cmake configuration file, where you can select the atan2 method you want to use
 */
rreal _atan2_approx_private(const rreal y, const rreal x) NOEXCEPT {
    // Because of how the algorithm works, w/ the union, if we have a double,
    // then we need to use a 64 bit unsigned integer then
    #ifdef USE_SINGLE_PRECISION_REAL
        constexpr int shift_by = 31;
        constexpr uint32_t sign_flip = 0x80000000u;
        union float_flipper {
            rreal f_val;
            uint32_t n_val;
        };
    #else
        constexpr int shift_by = 63;
        constexpr uint64_t sign_flip = 0x8000000000000000u;
        union float_flipper {
            rreal f_val;
            uint64_t n_val;
        };
    #endif

    // Algorithm adapted from: https://www.dsprelated.com/showarticle/1052.php
    // TODO [correctness]: are there some more precise numbers that could be used since
    //                     the original used `float` constants?
    constexpr auto n1 = static_cast<rreal>( 0.97239411);
    constexpr auto n2 = static_cast<rreal>(-0.19194795);

    if (x != 0) {
        const float_flipper ty_sign = {y};
        const float_flipper tx_sign = {x};

        if (std::abs(x) >= std::abs(y)) {
            const rreal z = y / x;
            float_flipper t_offset = {Pi};

            // Add or subtract PI based on y's sign
            t_offset.n_val |= ty_sign.n_val & sign_flip;

            // No offset if x is positive, so multiply by 0 or based on x's sign
            t_offset.n_val *= tx_sign.n_val >> shift_by;
            return t_offset.f_val + ((n1 + n2 * z * z) * z);
        } else {
            // Use atan(y/x) = pi/2 - atan(x/y) if |y/x| > 1
            const rreal z = x / y;
            float_flipper t_offset = {HalfPi};

            // Add or subtract PI/2 based on y's sign
            t_offset.n_val |= ty_sign.n_val & sign_flip;
            return t_offset.f_val - ((n1 + n2 * z * z) * z);
        }
    } else if (y > 0)
        return HalfPi;
    else if (y < 0)
        return -HalfPi;

    return 0;
}
#endif // !USE_BOOK_ATAN2



#ifndef USE_BOOK_ASIN
/*!
 * This uses a taylor series approximation (with error correction) to compute the arcsine
 * a bit faster.
 */
rreal _asin_approx_private(const rreal x) NOEXCEPT {
    // This uses a talor series approximation.
    // See: http://mathforum.org/library/drmath/view/54137.html

    // In the case where x=[-1, -0.8) or (0.8, 1.0] there is unfortunately a lot of
    // error compared to actual arcsine, so for this case we actually use the function
    constexpr rreal lim = 0.8;
    if ((x < static_cast<rreal>(-lim)) || (static_cast<rreal>(lim) < x))
        return std::asin(x);

    // The taylor series approximation
    // TODO [correctess] Could we add another layer of depth without a performance hit?
    constexpr auto a = static_cast<rreal>(0.5);
    constexpr auto b = static_cast<rreal>(a * 0.75);
    constexpr auto c = static_cast<rreal>(b * (5.0 / 6.0));
    constexpr auto d = static_cast<rreal>(c * (7.0 / 8.0));

    const auto aa = static_cast<rreal>((x * x * x) / 3.0);
    const auto bb = static_cast<rreal>((x * x * x * x * x) / 5.0);
    const auto cc = static_cast<rreal>((x * x * x * x * x * x * x) / 7.0);
    const auto dd = static_cast<rreal>((x * x * x * x * x * x * x * x * x) / 9.0);

    return x + (a * aa) + (b * bb) + (c * cc) + (d * dd);
}
#endif  // !USE_BOOK_ASIN



#ifndef USE_BOOK_SIN_COS
// Adapted from: http://www.ganssle.com/approx.htm
// NOTE: this function isn't needed unless we have the sine/cosine approximation use
//       so that's why it's in a `ifdef` block
inline rreal _cos_73s(const rreal x) NOEXCEPT {
    // TODO [correctness] See if the ones with more precision are just as efficient
    constexpr auto c1 = static_cast<rreal>( 0.999999953464);
    constexpr auto c2 = static_cast<rreal>(-0.499999053455);
    constexpr auto c3 = static_cast<rreal>( 0.0416635846769);
    constexpr auto c4 = static_cast<rreal>(-0.0013853704264);
    constexpr auto c5 = static_cast<rreal>( 0.00002315393167);
    const rreal x_sq = x * x;
    return c1 + (x_sq * (c2 + (x_sq * (c3 + (x_sq * (c4 + (c5 * x_sq)))))));
}

// This uses the taylor serires
rreal _cos_approx_private(rreal x) NOEXCEPT {
    // TODO [performance] This covers (0, 2 * Pi), but because of trig/cos identities, we only should need (0, 0.5 * Pi)
    //                    Instead, we should use thos equvilalencies to reduce the computations
    constexpr auto a = static_cast<rreal>(-1.0 / ( 2.0));
    constexpr auto b = static_cast<rreal>( 1.0 / ( 4.0 *  3.0 *  2.0));
    constexpr auto c = static_cast<rreal>(-1.0 / ( 6.0 *  5.0 *  4.0 *  3.0 *  2.0));
    constexpr auto d = static_cast<rreal>( 1.0 / ( 8.0 *  7.0 *  6.0 *  5.0 *  4.0 * 3.0 * 2.0));
    constexpr auto e = static_cast<rreal>(-1.0 / (10.0 *  9.0 *  8.0 *  7.0 *  6.0 * 5.0 * 4.0 * 3.0 * 2.0));
    constexpr auto f = static_cast<rreal>( 1.0 / (12.0 * 11.0 * 10.0 *  9.0 *  8.0 * 7.0 * 6.0 * 5.0 * 4.0 * 3.0 * 2.0));
    constexpr auto g = static_cast<rreal>(-1.0 / (14.0 * 13.0 * 12.0 * 11.0 * 10.0 * 9.0 * 8.0 * 7.0 * 6.0 * 5.0 * 4.0 * 3.0 * 2.0));

    // NOTE you'll see a lot dead code below.  After a bit of testing, I found the forth result
    //      to be the most performant on my machine.  I should double check with the generated
    //      assembly on godbolt though...
    x = fmod(x, TwoPi);                 // Modulo it to (0, 2Pi)
    x = x * x;                          // Square x
    const rreal x2 = x * x;             // (and then that too)
    const rreal x3 = x * x * x;         // (and then that three)
    const rreal x4 = x * x * x * x;     // (make it four!)

//    return 1 +
//           ((x) * a) +
//           ((x * x) * b) +
//           ((x * x * x) * c) +
//           ((x * x * x * x) * d) +
//           ((x * x * x * x * x) * e) +
//           ((x * x * x * x * x * x) * f) +
//           ((x * x * x * x * x * x * x) * g);

//    return 1 +
//           ((x) * a) +
//           ((x2) * b) +
//           ((x2 * x) * c) +
//           ((x2 * x2) * d) +
//           ((x2 * x2 * x) * e) +
//           ((x2 * x2 * x2) * f) +
//           ((x2 * x2 * x2 * x) * g);

//    return 1 +
//           ((x) * a) +
//           ((x2) * b) +
//           ((x3) * c) +
//           ((x3 * x) * d) +
//           ((x3 * x2) * e) +
//           ((x3 * x3) * f) +
//           ((x3 * x3 * x) * g);

    return 1 +
           ((x) * a) +
           ((x2) * b) +
           ((x3) * c) +
           ((x4) * d) +
           ((x4 * x) * e) +
           ((x4 * x2) * f) +
           ((x4 * x3) * g);
}

#endif  // !USE_BOOK_SIN_COS


#ifndef USE_BOOK_SQRT
// I've been trying to figure out a fast square root approximation and none of the tweaks I've made (including the
// babylonian method have actually performed better)... Right recommendation at the moment is to use std::sqrt()

// For the instrinsics method
//#include <x86intrin.h>

rreal _sqrt_approx_private(const rreal x) NOEXCEPT {
//    // Using x86 intrinsics
//    const __m128d reg = _mm_set1_pd(x);
//    return _mm_cvtsd_f64(_mm_sqrt_sd(reg, reg));


    // Uses method 7: https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi

    // TODO there is some loss of precision with the convertion from (possibly) double -> float.  There should be version
    //      of this approximation that works for 64 bit floats (i.e. doubles)

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Example
    //    rreal z = x / 2;
    //    for (int i = 0; i < 16; i++)
    //        z = 0.5 * (z + (x / z));
    //    return z;

    union twidler {
        float f_val;
        uint32_t i_val;
    };

    const auto f_x = static_cast<float>(x);
    twidler v = { f_x };
    v.i_val = (1 << 29) + (v.i_val >> 1) - (1 << 22);

    // Babylonian steps (more steps=more precision)
    for (int i = 0; i < 4; ++i)
        v.f_val = 0.5f * (v.f_val + f_x / v.f_val);

    return static_cast<rreal>(v.f_val);
}
#endif  // !USE_BOOK_SQRT


}   // util
