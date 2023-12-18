#pragma once

#include "Vec3.hpp"
#include <string>
#include <random>
#include "Util.hpp"

#ifndef USE_BOOK_RNG
    // We use the PCG random family to generate random numbers, it's faster
    #include "../third_party/pcg_random.hpp"
#endif


const std::string DefaultRNGSeed = "ASDF";

/*!
 * This is an RNG where you can plug in different types of distrubtions, generators
 * and types to generate.  It's not recommeded that you use this class directly,
 * unless you need to.  The reason that this exists is so that the randomly setup
 * scenes in the `Book*` can be setup the same, regardless if `rreal` is set to
 * `float` or `double` precision.
 *
 * Use the class `RandomGenerator` instead.
 */
template<template<class> class Distributor, class Type, class RNG>
class _GeneralizedRandomGenerator {
private:
    const std::string _RandomStringChars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Data
    Distributor<Type> _rng_distribution;
    std::seed_seq _seed;
    RNG _rng;

public:
    explicit _GeneralizedRandomGenerator(const std::string &rng_seed) NOEXCEPT :
        _rng_distribution(0, 1),
        _seed(rng_seed.begin(), rng_seed.end()),
        _rng(_seed)
    { }

    inline Type get_real() NOEXCEPT {
        return _rng_distribution(_rng);
    }

    inline Type get_real(const Type min, const Type max) NOEXCEPT {
        return min + ((max - min) * get_real());
    }

    inline int get_int(const int min, const int max) NOEXCEPT {
        return static_cast<int>(get_real(
            static_cast<Type>(min),
            static_cast<Type>(max + 1)
        ));
    }

    inline Vec3 get_vec3() NOEXCEPT {
        return Vec3(get_real(), get_real(), get_real());
    }

    inline Vec3 get_vec3(const Type lower, const Type upper) NOEXCEPT {
        return Vec3(
            get_real(lower, upper),
            get_real(lower, upper),
            get_real(lower, upper)
        );
    }

    inline Vec3 get_in_unit_sphere() NOEXCEPT {
        // BOOK CODE: (loop, with brancing, super bad... but it's acutely faster)
        while (true) {
            const Vec3 p = get_vec3(-1, 1);
            if (p.length_squared() >= 1)
                continue;

            return p;
        }

//        // This is adapted from https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
//        //   It was an attempt to generate the points in a more performant maner, but turned out to
//        //   be slightly slower, and a little different too.
//        const rreal r = std::cbrt(get_real(0, 1));
//        const rreal theta = get_real(0, TwoPi);
//        const rreal phi = /* std::acos(get_real(0, 2) - 1); */ HalfPi - util::asin(get_real(0, 2) - 1);       // Use our asin approximation with the acos trig identity
//
//        const rreal sin_theta = util::sin(theta);
//        const rreal cos_theta = util::cos(theta);
//        const rreal sin_phi = util::sin(phi);
//        const rreal cos_phi = util::cos(phi);
//
//        const rreal x = r * sin_phi * cos_theta;
//        const rreal y = r * sin_phi * sin_theta;
//        const rreal z = r * cos_phi;
//
//        return Vec3(x, y, z);
    }

    inline Vec3 get_in_unit_hemisphere(const Vec3 &normal) NOEXCEPT {
        const Vec3 in_unit_sphere = get_in_unit_sphere();
        return (in_unit_sphere.dot(normal) > 0) ? in_unit_sphere : -in_unit_sphere;
    }

    inline Vec3 get_in_unit_disk() NOEXCEPT {
        // BOOK CODE: (loop, with branching, super bad... but it's acutely faster)
        while (true) {
            const Vec3 p(get_real(-1, 1), get_real(-1, 1), 0);
            if (p.length_squared() >= 1)
                continue;

            return p;
        }

//        // This is polar, it's slightly slower (by 0.8%) for some odd reason...
//        const rreal r = util::sqrt(get_real(0, 1));
//        const rreal theta = get_real(0, TwoPi);
//
//        const rreal x = r * util::cos(theta);
//        const rreal y = r * util::sin(theta);
//
//        return Vec3(x, y, 0);
    }

    inline Vec3 get_unit_vector() NOEXCEPT {
        const Type a = get_real(0, TwoPi);
        const Type z = get_real(-1, 1);
        const rreal r = util::sqrt(1 - (z * z));

        return Vec3(r * util::cos(a), r * util::sin(a), z);
    }

    Vec3 get_cosine_direction() NOEXCEPT {
        const Type r1 = get_real();
        const Type r2 = get_real();
        const rreal phi = 2 * Pi * r1;
        const rreal z = util::sqrt(1 - r2);
        const rreal r2_sqrt = util::sqrt(r2);
        const rreal x = util::cos(phi) * r2_sqrt;
        const rreal y = util::sin(phi) * r2_sqrt;

        return Vec3(x, y, z);
    }

    Vec3 get_to_sphere(const Type radius, const Type distance_squared) {
        const Type r1 = get_real();
        const Type r2 = get_real();
        const rreal phi = 2 * Pi * r1;

        const rreal z = 1 + r2 * (util::sqrt(1 - (radius * radius / distance_squared)) - 1);
        const rreal zeta = util::sqrt(1 - (z * z));

        const rreal x = util::cos(phi) * zeta;
        const rreal y = util::sin(phi) * zeta;

        return Vec3(x, y, z);
    }


    // Returns a random string (of the requested size)
    // Will be populated with [0-9a-zA-Z]
    std::string get_random_string(const size_t num) NOEXCEPT {
        std::string str(num, '0');
        const int max = static_cast<int>(_RandomStringChars.size()) - 1;

        // Pick out the characters
        for (auto &s : str)
            s = _RandomStringChars[static_cast<size_t>(get_int(0, max))];

        return str;
    }
};



#ifdef USE_BOOK_RNG
    // The Book uses the built-in meresenne twister engine to generate random numbers, it's a little slower
    #define RNG_ENGINE std::mt19937
#else
    #define RNG_ENGINE pcg32
#endif


/*!
 * The random generator that should actually be used by the app in general places.
 */
class RandomGenerator : public _GeneralizedRandomGenerator<std::uniform_real_distribution, rreal, RNG_ENGINE> {
public:
    explicit RandomGenerator(const std::string &rng_seed) :
        _GeneralizedRandomGenerator(rng_seed)
    { }
};
