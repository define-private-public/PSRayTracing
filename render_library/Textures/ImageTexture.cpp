#include "ImageTexture.hpp"
#include "../third_party/stb_image.h"
#include "Util.hpp"
#include <stdexcept>
#include <algorithm>

using namespace std;


const int BytesPerPixel = 3;



ImageTexture::ImageTexture(const uint8_t *image_data, const int width, const int height, const int num_channels) :
    _width(width),
    _height(height),
    _bytes_per_scanline(num_channels * width)
{
    // Copy the pixels into the image buffer
    const auto num_bytes = static_cast<size_t>(width * height * num_channels);
    _img_data.assign(image_data, image_data + num_bytes);
}


// Note: I'm not too happy with how "duplicate" the two static `load_*()` methods are below, but I think it's
//       best to leave them like that for simplicty's scake


shared_ptr<ImageTexture> ImageTexture::load_from_file(const char *filename) {
    int num_channels = BytesPerPixel;
    int width, height;

    // Load image data
    uint8_t *tmp = stbi_load(filename, &width, &height, &num_channels, num_channels);
    if (!tmp)
        throw std::runtime_error("Error, couldn't load texture image from file");

    // Create texture
    auto texture = make_shared<ImageTexture>(tmp, width, height, num_channels);

    // Cleanup
    stbi_image_free(tmp);

    return texture;
}


shared_ptr<ImageTexture> ImageTexture::load_from_memory_buffer(const uint8_t *data, const int data_length) {
    int num_channels = BytesPerPixel;
    int width, height;

    // Load image data
    uint8_t *tmp = stbi_load_from_memory(data, data_length, &width, &height, &num_channels, num_channels);
    if (!tmp)
        throw std::runtime_error("Error, couldn't load texture image from memory buffer");

    // Create texture
    auto texture = make_shared<ImageTexture>(tmp, width, height, num_channels);

    // Cleanup
    stbi_image_free(tmp);

    return texture;
}


shared_ptr<ITexture> ImageTexture::deep_copy() const NOEXCEPT {
    return make_shared<ImageTexture>(*this);
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
