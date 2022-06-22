#include "RenderThreadPool.h"
#include "RandomGenerator.h"
#include "RenderOutput.h"
using namespace std;


RenderThreadPool::RenderThreadPool(const RenderContext &render_context, const uint16_t num_threads) NOEXCEPT :
    _r_ctx(render_context)
{
    // Create the threads
    for (uint16_t i = 0; i < num_threads; i++)
        _threads.push_back(RenderThread(_r_ctx));
}

void RenderThreadPool::setup_render(const string &main_rng_seed, const uint32_t samples_per_pixel, const uint16_t max_ray_depth)
{
    RandomGenerator rng(main_rng_seed);

    // Set which threads will render which scanlines
    RenderThread::Task task;
    task.samples_per_pixel = samples_per_pixel;
    task.max_ray_depth = max_ray_depth;

    const uint16_t height_max = _r_ctx.height - 1;
    auto row = static_cast<int16_t>(height_max);
    bool added = false;

    while (row >= 0)
    {
        // Setup the task
        task.scanline = static_cast<uint16_t>(row--);
        task.rng_seed = rng.get_random_string(16);

        // Pick which thread to add it to
        added = _threads[static_cast<size_t>(row) % _threads.size()].add_task(task);
        if (!added)
            throw runtime_error("Wasn't able to add render task");
    }
}

void RenderThreadPool::start_render()
{
    // Start all threads
    bool started = false;
    for (RenderThread &rt : _threads)
    {
        started = rt.run();
        if (!started)
            throw runtime_error("Wasn't able to start render thread");
    }
}

uint64_t RenderThreadPool::total_pixel_count() const NOEXCEPT
{
    uint64_t total = 0;
    for (const RenderThread &rt: _threads)
        total += rt.total_pixel_count();

    return total;
}

uint64_t RenderThreadPool::num_pixels_completed() const NOEXCEPT
{
    uint64_t completed = 0;
    for (const RenderThread &rt: _threads)
        completed += rt.num_pixels_completed();

    return completed;
}

// Checks if the render is done by looking at the task counts
bool RenderThreadPool::render_completed() const NOEXCEPT
{
    uint16_t tasks_done = 0;
    for (const RenderThread &rt: _threads)
        tasks_done += rt.num_tasks_completed();

    // the total number of tasks is equal to the height of the render
    return (tasks_done >= _r_ctx.height);
}

void RenderThreadPool::shutdown_and_wait() NOEXCEPT
{
    // Cleanly tell the threads to shutdown and wait for them all
    for (RenderThread &rt: _threads)
        rt.request_stop();

    for (RenderThread &rt: _threads)
        rt.stop_and_try_join();
}

// Write the results of the rendering to a byte buffer
void RenderThreadPool::retreive_render(const RenderOutput &render_desc, vector<uint8_t> &dest) NOEXCEPT
{
    RenderThread::Result result;
    bool got_scanline = false;

    // Loop through all the threads
    for (RenderThread &rt : _threads)
    {
        // And loop through all of their results
        got_scanline = rt.retrive_result(result);
        while (got_scanline)
        {
            write_rgb_scanline(render_desc, dest, result.scanline, result.data);

            // Try to get the next
            got_scanline = rt.retrive_result(result);
        }
    }
}
