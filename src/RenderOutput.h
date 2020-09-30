#pragma once

#include "Common.h"
#include <cstdint>
#include <cstddef>
#include <vector>
struct ColorRGBA;


// Small description of the image render
// TODO [refeactoring] maybe have this hold the pixel data as well?
//     then we could abstract away the stbi stuffs
struct RenderOutput {
    const uint16_t width;
    const uint16_t height;
    const uint8_t num_channels;

    // Caches of other values
    const uint32_t row_byte_size;
    const size_t total_byte_size;

    RenderOutput(const uint16_t width_, const uint16_t height_, const uint8_t num_channels_=4) NOEXCEPT :
        width(width_),
        height(height_),
        num_channels(num_channels_),
        row_byte_size(width * num_channels),
        total_byte_size(row_byte_size * height)
    { }
};

void write_rgb_scanline(const RenderOutput &ro, std::vector<uint8_t> &img, const uint16_t row, const std::vector<ColorRGBA> &pixels) NOEXCEPT;
