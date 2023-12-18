#pragma once

#include "Common.hpp"
#include <memory>


/*!
    Implementing this means that a deep copy of the object (including pointers) can be
    produced by the method `deep_copy()`
*/
template<class T>
class IDeepCopyable {
public:
    virtual ~IDeepCopyable() NOEXCEPT = default;

    virtual std::shared_ptr<T> deep_copy() const NOEXCEPT = 0;
};
