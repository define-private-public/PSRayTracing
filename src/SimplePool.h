#pragma once

#include "Common.h"
#include <vector>

constexpr size_t DefaultInitialPoolSize = 1000; // TODO should be power of 2
constexpr size_t DefaultMaxPoolSize = 100000;   // TODO should be power of 2


// TODO document
template<class Type>
class SimplePool {
private:
    // Pool data
    size_t _next = 0;
    size_t _next_pool_size = 0;
    const size_t _max_pool_size;
    std::vector<Type> _pool;

public:
    SimplePool(
        const size_t initial_pool_size=DefaultInitialPoolSize,
        const size_t max_pool_size=DefaultMaxPoolSize
    ) NOEXCEPT :
        _next_pool_size(initial_pool_size),
        _max_pool_size(max_pool_size)
    {};

    inline Type get_next() NOEXCEPT{
        // First make sure we haven't exhausted the pool.  If we have, then realloc
        if (_next >= _pool.size())
            _realloc_pool_and_reset_cursor();

        // Now return the new object
        return _pool[_next++];
    }

private:
    void _realloc_pool_and_reset_cursor() NOEXCEPT {/* TODO */};

};