#include "ONB.h"

using namespace std;


void ONB::build_from_w(const Vec3 &n) NOEXCEPT {
    axis[2] = n.unit_vector();

    const Vec3 a = (fabs(w().x) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);

    axis[1] = w().cross(a).unit_vector();
    axis[0] = w().cross(v());
}
