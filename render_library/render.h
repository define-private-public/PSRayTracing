// This is the interface to use the PSRayTracing code as a library

#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <future>


namespace psrt {


/** A descriptor of how a render should be performed */
struct RenderJob
{
    uint16_t render_width       = 960;
    uint16_t render_height      = 540;
    std::string scene_id        = "book2::final_scene";
    uint32_t samples_per_pixel  = 10;
    uint16_t max_depth          = 50;
    uint16_t num_threads        = 1;
    std::string seed_str        = "ASDF";
    bool deep_copy_per_thread   = true;
};


/** The result of a render job. */
struct Render
{
    std::vector<uint8_t> image_data;
    uint16_t render_width = 0;
    uint16_t render_height = 0;
    uint8_t num_channels = 0;
    float render_time_in_seconds = 0.0f;
    std::chrono::nanoseconds render_time_in_nanoseconds;

    bool was_succesful() const
        { return !image_data.empty(); }
};


// The fuctions to perform a render
std::future<Render> do_render(const RenderJob &job);    ///< Will start a render job (if one isn't in progress)
void stop_active_render();                              ///< Will stop a render job (if one is in progress)
float render_progress();                                ///< Returns the progress of a render [0.0, 1.0].  Returns less than 0 (e.g. -1) if no render is happening
bool render_in_progress();                              ///< Checks to see if a render is already in progress (or not)
std::vector<std::string> all_scene_ids();               ///< Retrives all possible scenes that could be rendered
int num_concurrent_threads_supported();                 ///< Retrives the maximum number of threads that can ru concurrently


}   // psrt::
