#pragma once

#include <vector>
#include "RenderContext.hpp"
#include "RenderThread.hpp"
struct RenderOutput;


// A class that manages multiple render threads, and getting everying done for you
class RenderThreadPool
{
private:
    // Data
    RenderContext _r_ctx;
    std::vector<RenderThread> _threads;

public:
    explicit RenderThreadPool(const RenderContext &render_context, const uint16_t num_threads) NOEXCEPT;

    void setup_render(const std::string &main_rng_seed, const uint32_t samples_per_pixel, const uint16_t max_ray_depth);
    void start_render();
    void shutdown_and_wait() NOEXCEPT;

    uint64_t total_pixel_count() const NOEXCEPT;
    uint64_t num_pixels_completed() const NOEXCEPT;
    bool render_completed() const NOEXCEPT;
    void retreive_render(const RenderOutput &render_desc, std::vector<uint8_t> &dest) NOEXCEPT;
};
