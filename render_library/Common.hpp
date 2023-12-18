#pragma once

// A collection of things (most likely defines or datatypes) that should be common across the project
#include <limits>

// adding `noexcept` to your functions usually results in a perf. boost.  It's here as a toggable flag
//   to show the effects of it happening (or not)
#ifdef USE_NOEXCEPT
    #define NOEXCEPT noexcept
#else
    #define NOEXCEPT
#endif


#ifdef USE_SINGLE_PRECISION_REAL
    // Use floats (singles) for the maths
    using rreal = float;
#else
    // Use doubles for the maths
    using rreal = double;
#endif


constexpr auto Infinity = std::numeric_limits<rreal>::infinity();
constexpr auto Pi = static_cast<rreal>(3.1415926535897932385);
constexpr rreal HalfPi = Pi / static_cast<rreal>(2);
constexpr rreal TwoPi = static_cast<rreal>(2) * Pi;
constexpr rreal ThreeHalvesPi = static_cast<rreal>(3.0 / 2.0) * Pi;
constexpr rreal InvTwoPi = static_cast<rreal>(1) / TwoPi;
constexpr rreal TwoOverPi = static_cast<rreal>(2) / Pi;
