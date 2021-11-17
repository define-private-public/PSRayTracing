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
    /** Creates an image texture with the raw pixel data passed into \a image_data. */
    explicit ImageTexture(const uint8_t *image_data, const int width, const int height, const int num_channels);

    /** Loads an image texture from the given path at \a filename */
    static std::shared_ptr<ImageTexture> load_from_file(const char *filename);

    /**
     * Loads an image texture from a memory buffer (e.g. an embedded resource)
     *
     * \a data is not the raw pixel data, but encoded in a format like JPEG, PNG, etc.
     */
    static std::shared_ptr<ImageTexture> load_from_memory_buffer(const uint8_t *data, const int data_length);

    std::shared_ptr<ITexture> deep_copy() const NOEXCEPT override;

    Vec3 value(const rreal u, const rreal v, const Vec3 &p) const NOEXCEPT override;
};
