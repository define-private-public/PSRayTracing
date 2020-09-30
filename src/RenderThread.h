#pragma once

// Based off of work here:
//   https://www.bfilipek.com/2019/12/threading-loopers-cpp17.html

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <vector>
#include <random>
#include "Vec3.h"
#include "ColorRGBA.h"
class IHittable;
class ICamera;
class Ray;
class RandomGenerator;
struct RenderOutput;

// TODO [features]
// - RenderTask should be per-pixel not per scanline
//       in the future, maybe add some fields to start from an offset, and only render `n` pixels?
//       that way we can render a patch, or a sub-scanline.  The issue arrises with the RNG though, if
//       it doesn't render some pixels first, the RNG is going to start differnetly for that fist pixel than
//       it normally would...  Maybe a solution would be to have more random seeds, and each pixels has it's own seed
// - RenderThread isn't flexable
//       that if you want to change out what's being rendered, you need to destory
//       the thread(s) and create a fully brand new object with a different context.
//       So things honestly aren't that reusuable, which is kind of sad


// The general context of what needs to be rendered
//   Treat this as a read-only structure
struct RenderContext {
    // Data
    std::shared_ptr<IHittable> scene = nullptr; // Objects to render
    std::shared_ptr<ICamera> camera = nullptr;  // Camera to render with
    Vec3 background = Vec3(0);                  // Background of scene, affects the illumination
    uint16_t width = 1;                         // Render image width
    uint16_t height = 1;                        // Render image height
};

// A job of what to render, right now a single RenderThread will work on a scanline at a time
//   Keep this a read-only too
struct RenderTask {
    // Data
    uint16_t scanline = 0;              // Which no. scanline of the render to work on
    uint16_t max_ray_depth = 50;        // Maximum number of ray bouncies
    uint32_t samples_per_pixel = 1;     // How many times to sample a pixel

    std::string rng_seed;               // Seed for RNG for this task, can be any fun phrase you want!
};

// Right now this is per-scanline, if we make it perpixel, or allow sub scanlines it will need to be
// more rhobust.  See the above
struct RenderResult {
    // Data
    uint16_t scanline = 0;              // The scanline that was rendered
    std::vector<ColorRGBA> data{};      // pixels
};


// A thread object to queue up render work
class RenderThread {
private:
    // Data
    RenderContext _r_ctx;

    // Threading
    std::thread _thread;
    std::atomic_bool _running;
    std::atomic_bool _stop_requested;

    // Work queuing
    std::mutex _task_queue_mutex;
    std::queue<RenderTask> _task_queue;

    // Results
    std::mutex _result_queue_mutex;
    std::queue<RenderResult> _result_queue;
    std::atomic_uint16_t _completed_task_count;

    // Metering
    std::atomic_uint64_t _total_queued_pixels;  // How many pixels in total will be worked on
    std::atomic_uint64_t _pixels_completed;     // How many pixels have been completed thus far

public:
    RenderThread(const RenderContext &render_context) NOEXCEPT;
    RenderThread(RenderThread &&other) noexcept;

    bool run() NOEXCEPT;

    // Queue a task to turn, returns false if failed to queue
    bool add_task(const RenderTask &task) NOEXCEPT;

    // Try to get a result, returns false if no result was placed in `result`
    bool retrive_result(RenderResult &result) NOEXCEPT;

    inline uint16_t num_tasks_completed() const NOEXCEPT {
       return _completed_task_count.load();
    }

    inline uint64_t total_pixel_count() const NOEXCEPT {
        return _total_queued_pixels.load();
    }

    inline uint64_t num_pixels_completed() const NOEXCEPT {
        return _pixels_completed.load();
    }

    inline bool running() const NOEXCEPT {
        return _running.load();
    }

    inline void request_stop() NOEXCEPT {
        _stop_requested.store(true);
    }

    inline void stop_and_try_join() NOEXCEPT {
        request_stop();
        if (_thread.joinable())
            _thread.join();
    }

private:
    // == Thread functions
    void _thread_main_loop() NOEXCEPT;                                          // Main loop of the thread
    std::vector<ColorRGBA> _render_scanline(const RenderTask &task) NOEXCEPT;   // Called in the main loop, where rendering actually takes place

    // Attempts to get a task from the work queue
    //   return `true` if a task was pulled off.  It's then placed into `task`.
    //           if `false`, then one wasn't pulled off.  `task` shouldn't be modified then
    bool _get_next_task(RenderTask &task) NOEXCEPT;

    // Place the result of a render into a queue for the thread owner to receive
    bool _store_result(const uint16_t scanline, const std::vector<ColorRGBA> &data) NOEXCEPT;
};



// A class that manages multiple render threads, and getting everying done for you
class RenderThreadPool {
private:
    // Data
    RenderContext _r_ctx;
    std::vector<RenderThread> _threads;

public:
    RenderThreadPool(const RenderContext &render_context, const uint16_t num_threads) NOEXCEPT;

    void setup_render(const std::string &main_rng_seed, const uint32_t samples_per_pixel, const uint16_t max_ray_depth);
    void start_render();
    void shutdown_and_wait() NOEXCEPT;

    uint64_t total_pixel_count() const NOEXCEPT;
    uint64_t num_pixels_completed() const NOEXCEPT;
    bool render_completed() const NOEXCEPT;
    void retreive_render(const RenderOutput &render_desc, std::vector<uint8_t> &dest) NOEXCEPT;
};



// the funtion where the rendering magic happens
Vec3 ray_color(const RenderContext &r_ctx, RandomGenerator &rng, const Ray &r, const uint32_t depth) NOEXCEPT;
Vec3 ray_color_iterative(const RenderContext &r_ctx, RandomGenerator &rng, const Ray &initial_ray, const uint32_t depth) NOEXCEPT;


// For an experiment in batching processing to exploit branch prediction
//struct BatchedVec3 {
//    size_t id;
//    Vec3 v;
//};
//
//struct BatchedRay {
//    size_t id;
//    Ray ray;
//};

//Vec3 ray_color_iterative_batched(const RenderContext &r_ctx, RandomGenerator &rng, const std::vector<Ray> &rays, const uint32_t depth);

/*
std::vector<BatchedVec3> ray_color_batched(const RenderContext &r_ctx, RandomGenerator &rng, const std::vector<BatchedRay> &rays, const uint32_t depth);
*/