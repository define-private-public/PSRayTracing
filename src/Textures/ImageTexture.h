#pragma once

#include "Interfaces/ITexture.h"
#include <cstdint>
#include <vector>


class ImageTexture : public ITexture {
private:
    // Data
    std::vector<uint8_t> _img_data;
    int _width = 0, _height = 0;
    int _bytes_per_scanline = 0;

public:
    explicit ImageTexture(const char *filename);

    virtual Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;

};
