#include "SimplePool.h"

/*
template<class Type>
void SimplePool<Type>::_realloc_pool_and_reset_cursor() NOEXCEPT {
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
*/
