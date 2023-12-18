#pragma once

// Based off of work here:
//   https://www.bfilipek.com/2019/12/threading-loopers-cpp17.html

#include <thread>
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>
#include <vector>
#include <random>
#include <array>
#include "Vec3.hpp"
#include "ColorRGBA.hpp"
#include "RenderContext.hpp"
class IHittable;
class ICamera;
class Ray;
class RandomGenerator;
struct RenderOutput;



/** A thread object to queue up render work */
class RenderThread
{
public:
    /*== Data Structures ==*/
    /**
     * A job of what to render, right now a single RenderThread will work on a scanline at a time.
     * Keep this as a read-only structure
     */
    struct Task
    {
        uint16_t scanline = 0;              ///< Which no. scanline of the render to work on
        uint16_t max_ray_depth = 50;        ///< Maximum number of ray bouncies
        uint32_t samples_per_pixel = 1;     ///< How many times to sample a pixel

        std::string rng_seed;               ///< Seed for RNG for this task, can be any fun phrase you want!
    };

    /**
     * Right now this is per-scanline, if we make it per-pixel, or allow sub scanlines it will need to be
     * more robust.  See the above `Task`.
     */
    struct Result
    {
        uint16_t scanline = 0;              ///< The scanline that was rendered
        std::vector<ColorRGBA> data{};      ///< pixels
    };

private:
    /*== Data ==*/
    RenderContext _r_ctx;

    // Threading
    std::thread _thread;
    std::atomic_bool _running;
    std::atomic_bool _stop_requested;

    // Work queuing
    std::mutex _task_queue_mutex;
    std::queue<Task> _task_queue;

    // Results
    std::mutex _result_queue_mutex;
    std::queue<Result> _result_queue;
    std::atomic_uint16_t _completed_task_count;

    // Metering
    std::atomic_uint64_t _total_queued_pixels;  // How many pixels in total will be worked on
    std::atomic_uint64_t _pixels_completed;     // How many pixels have been completed thus far

public:
    explicit RenderThread(const RenderContext &render_context) NOEXCEPT;
    explicit RenderThread(RenderThread &&other) NOEXCEPT;

    bool run() NOEXCEPT;

    // Queue a task to turn, returns false if failed to queue
    bool add_task(const Task &task) NOEXCEPT;

    // Try to get a result, returns false if no result was placed in `result`
    bool retrive_result(Result &result) NOEXCEPT;

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
    std::vector<ColorRGBA> _render_scanline(const Task &task) NOEXCEPT;   // Called in the main loop, where rendering actually takes place

    // Attempts to get a task from the work queue
    //   return `true` if a task was pulled off.  It's then placed into `task`.
    //           if `false`, then one wasn't pulled off.  `task` shouldn't be modified then
    bool _get_next_task(Task &task) NOEXCEPT;

    // Place the result of a render into a queue for the thread owner to receive
    bool _store_result(const uint16_t scanline, const std::vector<ColorRGBA> &data) NOEXCEPT;
};
