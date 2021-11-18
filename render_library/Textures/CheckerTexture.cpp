#include "CheckerTexture.h"
#include "SolidColor.h"
#include "Util.h"

using namespace std;


CheckerTexture::CheckerTexture(const shared_ptr<ITexture> &even, const shared_ptr<ITexture> &odd) NOEXCEPT :
    _even(even),
    _odd(odd)
{ }

CheckerTexture::CheckerTexture(const Vec3 &even_clr, const Vec3 &odd_clr) NOEXCEPT :
    CheckerTexture(
        make_shared<SolidColor>(even_clr),
        make_shared<SolidColor>(odd_clr)
    )
{ }

shared_ptr<ITexture> CheckerTexture::deep_copy() const NOEXCEPT {
    // Make the deepy copy
    auto ct = make_shared<CheckerTexture>(*this);
    ct->_even = _even->deep_copy();
    ct->_odd = _odd->deep_copy();

    return ct;
}

Vec3 CheckerTexture::value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT {
    const rreal sines = util::sin(x_frequency * p.x)
                      * util::sin(y_frequency * p.y)
                      * util::sin(z_frequency * p.z);
    if (sines < 0)
        return _odd->value(u, v, p);
    else
        return _even->value(u, v, p);
}
