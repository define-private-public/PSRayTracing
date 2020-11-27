#pragma once

#include "Common.h"
//#include <vector>
#include <functional>
#include <random>

#ifndef USE_BOOK_RNG
    // We use the PCG random family to generate random numbers, it's faster
    #include "third_party/pcg_random.hpp"
#endif


constexpr size_t DefaultPoolSize = 65536;



// TODO document
template<class Type, size_t pool_size>
class CachedPool {
private:
//    std::vector<Type> _pool;                // TODO this could be a flat array instead, using a template type
    Type _pool[pool_size];
    std::function<Type()> _alloc_func;

    std::uniform_int_distribution<size_t> _selector;
    RNG_ENGINE _rng;

public:
    CachedPool(
        std::function<Type()> obj_alloc_func
    ) NOEXCEPT :
        _alloc_func(obj_alloc_func),
        _selector(0, (pool_size - 1))
    {
        static_assert(pool_size != 0, "Pool must at least have something in it...");

        // Allocate all the values for the pool
        for (size_t i = 0; i < pool_size; i++)
            _pool[i] = _alloc_func();
    }

    // TODO document
    inline Type get_next() NOEXCEPT{
        return _pool[_selector(_rng)];
    }
};

