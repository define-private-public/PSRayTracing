#pragma once

#include "Vec3.h"
#include <string>
#include <random>
#include "Util.h"

#ifndef USE_BOOK_RNG
    // We use the PCG random family to generate random numbers, it's faster
    #include "third_party/pcg_random.hpp"
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

        // BOOK CODE: (loop, super bad...)
        while (true) {
            Vec3 p = get_vec3(-1, 1);
            if (p.length_squared() >= 1)
                continue;

            return p;
        }


/*
        // This isn't working either, it's coming out more specular than the book code
        // Generate a vector in spherical coordinates
        const Type r = get_real(0, 1);
        const Type theta = get_real(0, Pi);
        const Type phi = get_real(0, TwoPi);

        // Convert to the cartesian space
        return Vec3(
            r * util::sin(theta) * util::cos(phi),
            r * util::sin(theta) * util::sin(phi),
            r * util::cos(theta)
        );
        */
    }

    inline Vec3 get_in_unit_hemisphere(const Vec3 &normal) NOEXCEPT {
        const Vec3 in_unit_sphere = get_in_unit_sphere();
        return (in_unit_sphere.dot(normal) > 0) ? in_unit_sphere : -in_unit_sphere;
    }

    inline Vec3 get_in_unit_disk() NOEXCEPT {
        // BOOK CODE: (loop, super bad...)
        while (true) {
            Vec3 p(get_real(-1, 1), get_real(-1, 1), 0);
            if (p.length_squared() >= 1)
                continue;

            return p;
        }
    }

    inline Vec3 get_unit_vector() NOEXCEPT {
        const Type a = get_real(0, TwoPi);
        const Type z = get_real(-1, 1);
        const Type r = util::sqrt(1 - (z * z));

        return Vec3(r * util::cos(a), r * util::sin(a), z);
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
