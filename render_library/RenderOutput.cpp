#include "RenderOutput.h"
#include "ColorRGBA.h"
#include "Util.h"

using namespace std;
using namespace util;


// Write an entire scanline of pixels to the image
void write_rgb_scanline(const RenderOutput &rd, vector<uint8_t> &img, const uint16_t row, const vector<ColorRGBA> &pixels) NOEXCEPT {
    uint32_t loc = rd.row_byte_size * row;

    for (const ColorRGBA &rgba: pixels) {
        // Convert the RGBA to the pixel and write it
        img[loc++] = static_cast<uint8_t>(256 * clamp(rgba.r, 0.0, static_cast<rreal>(0.999)));
        img[loc++] = static_cast<uint8_t>(256 * clamp(rgba.g, 0.0, static_cast<rreal>(0.999)));
        img[loc++] = static_cast<uint8_t>(256 * clamp(rgba.b, 0.0, static_cast<rreal>(0.999)));
        img[loc++] = static_cast<uint8_t>(256 * clamp(rgba.a, 0.0, static_cast<rreal>(0.999)));
    }
}

