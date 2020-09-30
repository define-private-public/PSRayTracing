#include "ImageTexture.h"
#include "third_party/stb_image.h"
#include "Util.h"
#include <stdexcept>
#include <algorithm>


const int BytesPerPixel = 3;


ImageTexture::ImageTexture(const char *filename) {
    int num_channels = BytesPerPixel;

    // Load into a temporary location
    uint8_t *tmp = stbi_load(filename, &_width, &_height, &num_channels, num_channels);
    if (!tmp)
        throw std::runtime_error("Error, couldn't load texture image file");

    // Copy into a vector
    const auto num_bytes = static_cast<size_t>(_width * _height * num_channels);
    _img_data.reserve(num_bytes);
    copy(tmp, tmp + num_bytes, _img_data.begin());

    _bytes_per_scanline = num_channels * _width;

    stbi_image_free(tmp);
}

Vec3 ImageTexture::value(
    const rreal u,
    const rreal v,
    [[maybe_unused]] const Vec3 &p
) const NOEXCEPT {

    // Clamp input texture coordinates to [0,1] x [1,0]
    const rreal u2 = util::clamp(u, 0, 1);
    const rreal v2 = 1 - util::clamp(v, 0, 1);          // Flip V to image coordinates

    auto i = static_cast<int>(u2 * static_cast<rreal>(_width));
    auto j = static_cast<int>(v2 * static_cast<rreal>(_height));

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    i = std::min(i, _width - 1);
    j = std::min(j, _height - 1);

    const auto colour_scale = static_cast<rreal>(1.0 / 255.0);
    const auto offset = static_cast<size_t>((j * _bytes_per_scanline) + (i * BytesPerPixel));

    return Vec3(
        colour_scale * _img_data[offset + 0],
        colour_scale * _img_data[offset + 1],
        colour_scale * _img_data[offset + 2]
    );
}
