#pragma once

#include "Common.h"
#include <vector>
#include <functional>
#include <iostream>

constexpr size_t DefaultInitialPoolSize = 4096;
constexpr size_t DefaultMaxPoolSize = 65536;


// TODO document
template<class Type>
class SimplePool {
private:
    // Pool data (TODO document)
    size_t _next = 0;
    size_t _next_pool_size = 0;
    const size_t _max_pool_size;
    std::vector<Type> _pool;
    std::function<Type()> _alloc_func;

public:
    SimplePool(
        std::function<Type()> obj_alloc_func,
        const size_t initial_pool_size=DefaultInitialPoolSize,
        const size_t max_pool_size=DefaultMaxPoolSize
    ) NOEXCEPT :
        _next_pool_size(initial_pool_size),
        _max_pool_size(max_pool_size),
        _alloc_func(obj_alloc_func)
    {
        _realloc_pool_and_reset_cursor();
    };

    inline Type get_next() NOEXCEPT{
        // First make sure we haven't exhausted the pool.  If we have, then realloc
        if (_next >= _pool.size())
            _realloc_pool_and_reset_cursor();

        // Now return the new object
        return _pool[_next++];
    }

private:
    void _realloc_pool_and_reset_cursor() NOEXCEPT {
        // Clear out the old pool (and realloc)
        _pool.clear();
        _pool.reserve(_next_pool_size);

        // Call the allocation function until we have enough of what we need
        for (size_t i = 0; i < _next_pool_size; i++)
            _pool.emplace_back(_alloc_func());

        // Reset the cursor
        _next = 0;

        // For next time
        _next_pool_size = std::min(_next_pool_size * 2, _max_pool_size);
    }

};
