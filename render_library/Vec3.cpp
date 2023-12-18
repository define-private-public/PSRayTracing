#include "Vec3.hpp"
#include <iostream>


std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ')';
}
