#include "RenderThread.h"
#include "Interfaces/IHittable.h"
#include "Interfaces/ICamera.h"
#include "Interfaces/IMaterial.h"
#include "Ray.h"
#include "Util.h"
#include "HitRecord.h"
#include "RenderOutput.h"
#include "RandomGenerator.h"
using namespace std;


// TODO [features]
// - In RenderThread:_thread_main_loop():
//       what if a shutdown was requested mid-task? It's not able to shutdown until a task is done


// Finds what the color should be for the given pixel
ColorRGBA _pixel_color(const RenderContext &r_ctx, RandomGenerator &rng, const uint32_t samples_per_pixel, const rreal x, const rreal y, const uint16_t max_ray_depth) NOEXCEPT;


RenderThread::RenderThread(const RenderContext &render_context) NOEXCEPT :
    _r_ctx(render_context)
{
    _running.store(false);
    _stop_requested.store(false);
    _completed_task_count.store(0);

    _total_queued_pixels.store(0);
    _pixels_completed.store(0);
}

RenderThread::RenderThread(RenderThread &&other) noexcept :
    _r_ctx(move(other._r_ctx)),
    _thread(move(other._thread)),
    _task_queue(move(other._task_queue)),
    _result_queue(move(other._result_queue))
{
    // Can't move the atmoics
    _running.store(other._running.load());
    _stop_requested.store(other._stop_requested.load());
    _completed_task_count.store(other._completed_task_count.load());
    _total_queued_pixels.store(other._total_queued_pixels.load());
    _pixels_completed.store(other._pixels_completed.load());
}

// Returns true if the thread was successfuly created and starts running
bool RenderThread::run() NOEXCEPT {
#ifdef USE_EXCEPTIONS
    try {
#endif

        // Create the thread object
        _thread = thread(&RenderThread::_thread_main_loop, this);

#ifdef USE_EXCEPTIONS
    } catch (...) {
        return false;
    }
#endif

    return true;
}

bool RenderThread::add_task(const RenderTask &task) NOEXCEPT {
#ifdef USE_EXCEPTIONS
    try {
#endif

        {
            // Put the task on
            lock_guard guard(_task_queue_mutex);
            _task_queue.push(task);
        }

        // Store how many pixels need to be completed in total
        _total_queued_pixels.store(_total_queued_pixels.load() + _r_ctx.width);

        return true;

#ifdef USE_EXCEPTIONS
    } catch (...) {
        return false;
    }
#endif
}

bool RenderThread::retrive_result(RenderResult &result) NOEXCEPT {
    lock_guard guard(_result_queue_mutex);

    // Nothing?
    if (_result_queue.empty())
        return false;

    // Else, got something, grab it and place it
    result = _result_queue.front();
    _result_queue.pop();
    return true;
}

void RenderThread::_thread_main_loop() NOEXCEPT {
    _running.store(true);
    _stop_requested.store(false);

    // Deep copy the scene, that way each thread has it's own local version to use
    if (_r_ctx.deep_copy_per_thread) {
        _r_ctx.camera = _r_ctx.camera->deep_copy();
        _r_ctx.scene = _r_ctx.scene->deep_copy();
    }

    RenderTask task;
    bool got_task = false;

    while (_stop_requested.load() != true) {

#ifdef USE_EXCEPTIONS
        try {
#endif

            // Do render here

            got_task = _get_next_task(task);
            if (got_task)
                _store_result(task.scanline, _render_scanline(task));
            else
                this_thread::sleep_for(chrono::milliseconds(10));   // Take a short nap

#ifdef USE_EXCEPTIONS
        } catch (...) {
            // No errors leave thread
        }
#endif

    }

    _running.store(false);
}

vector<ColorRGBA> RenderThread::_render_scanline(const RenderTask &task) NOEXCEPT {
    const auto scanline_num = static_cast<rreal>(task.scanline);
    vector<ColorRGBA> scanline(static_cast<size_t>(_r_ctx.width));
    RandomGenerator rng(task.rng_seed);

    for (uint16_t x = 0; x < _r_ctx.width; ++x) {
        // Put the pixel into the scanline
        scanline[x] = _pixel_color(_r_ctx, rng, task.samples_per_pixel, x, scanline_num, task.max_ray_depth);

        // Metering
        _pixels_completed.store(_pixels_completed.load() + 1);
    }

    return scanline;
}

bool RenderThread::_get_next_task(RenderTask &task) NOEXCEPT {
    lock_guard guard(_task_queue_mutex);

    // Nothing?
    if (_task_queue.empty())
        return false;

    // There's something, grab it and place it
    task = _task_queue.front();
    _task_queue.pop();
    return true;
}

bool RenderThread::_store_result(const uint16_t scanline, const std::vector<ColorRGBA> &data) NOEXCEPT {
#ifdef USE_EXCEPTIONS
    try {
#endif

        const uint16_t num_completed_now = _completed_task_count.load() + 1;
        RenderResult rr;
        rr.scanline = scanline;
        rr.data = data;

        // Put it onto the result queue (and notify)
        lock_guard guard(_result_queue_mutex);
        _result_queue.push(rr);
        _completed_task_count.store(num_completed_now);

        return true;

#ifdef USE_EXCEPTIONS
    } catch (...) {
        return false;
    }
#endif
}



RenderThreadPool::RenderThreadPool(const RenderContext &render_context, const uint16_t num_threads) NOEXCEPT :
    _r_ctx(render_context)
{
    // Create the threads
    for (uint16_t i = 0; i < num_threads; i++)
        _threads.push_back(RenderThread(_r_ctx));
}

void RenderThreadPool::setup_render(const string &main_rng_seed, const uint32_t samples_per_pixel, const uint16_t max_ray_depth) {
    RandomGenerator rng(main_rng_seed);

    // Set which threads will render which scanlines
    RenderTask task;
    task.samples_per_pixel = samples_per_pixel;
    task.max_ray_depth = max_ray_depth;

    const uint16_t height_max = _r_ctx.height - 1;
    auto row = static_cast<int16_t>(height_max);
    [[maybe_unused]] bool added = false;

    while (row >= 0) {
        // Setup the task
        task.scanline = static_cast<uint16_t>(row--);
        task.rng_seed = rng.get_random_string(16);

        // Pick which thread to add it to
        added = _threads[static_cast<size_t>(row) % _threads.size()].add_task(task);

#ifdef USE_EXCEPTIONS
        if (!added)
            throw runtime_error("Wasn't able to add render task");
#endif
    }
}

void RenderThreadPool::start_render() {
    // Start all threads
    [[maybe_unused]] bool started = false;
    for (RenderThread &rt : _threads) {
        started = rt.run();

#ifdef USE_EXCEPTIONS
        if (!started)
            throw runtime_error("Wasn't able to start render thread");
#endif
    }
}

uint64_t RenderThreadPool::total_pixel_count() const NOEXCEPT {
    uint64_t total = 0;
    for (const RenderThread &rt: _threads)
        total += rt.total_pixel_count();

    return total;
}

uint64_t RenderThreadPool::num_pixels_completed() const NOEXCEPT {
    uint64_t completed = 0;
    for (const RenderThread &rt: _threads)
        completed += rt.num_pixels_completed();

    return completed;
}

// Checks if the render is done by looking at the task counts
bool RenderThreadPool::render_completed() const NOEXCEPT {
    uint16_t tasks_done = 0;
    for (const RenderThread &rt: _threads)
        tasks_done += rt.num_tasks_completed();

    // the total number of tasks is equal to the height of the render
    return (tasks_done >= _r_ctx.height);
}

void RenderThreadPool::shutdown_and_wait() NOEXCEPT {
    // Cleanly tell the threads to shutdown and wait for them all
    for (RenderThread &rt: _threads)
        rt.request_stop();

    for (RenderThread &rt: _threads)
        rt.stop_and_try_join();
}

// Write the results of the rendering to a byte buffer
void RenderThreadPool::retreive_render(const RenderOutput &render_desc, vector<uint8_t> &dest) NOEXCEPT {
    RenderResult result;
    bool got_scanline = false;

    // Loop through all the threads
    for (RenderThread &rt : _threads) {
        // And loop through all of their results
        got_scanline = rt.retrive_result(result);
        while (got_scanline) {
            write_rgb_scanline(render_desc, dest, result.scanline, result.data);

            // Try to get the next
            got_scanline = rt.retrive_result(result);
        }
    }
}



Vec3 ray_color(const RenderContext &r_ctx, RandomGenerator &rng, const Ray &r, const uint32_t depth) NOEXCEPT {
    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return Vec3(0);

    HitRecord rec{};

    // Anything?
    const bool hit_something = r_ctx.scene->hit(rng, r, static_cast<rreal>(0.001), Infinity, rec);
    if (!hit_something)
        return r_ctx.background;

    // Hit something...
    Ray scattered;
    Vec3 attenuation;
    const Vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    const bool rays_scattered = rec.mat_ptr->scatter(rng, r, rec, attenuation, scattered);

    // If hit a an emmsive material (they don't scatter rays), then only emit that one's light
    if (!rays_scattered)
        return emitted;

    // else, it must have been non-emissive,
    return emitted + (attenuation * ray_color(r_ctx, rng, scattered, depth - 1));
}

/*
struct BatchedHitRecord {
    size_t id;
    HitRecord rec;
    bool hit_something;
};

vector<BatchedVec3> ray_color_batched(const RenderContext &r_ctx, RandomGenerator &rng, const std::vector<BatchedRay> &rays, const uint32_t depth) {
    const size_t num_rays = rays.size();

    // Depending upon how many input rays we got, create the returns, make sure they have the same id's
    vector<BatchedVec3> vecs(num_rays);
    for (size_t s = 0; s < num_rays; ++s)
        vecs[s].id = rays[s].id;

    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0) {
        for (auto &v : vecs)
            v.v = Vec3(0);

        return vecs;
    }

    // Make sure IDs are the same
    vector<BatchedHitRecord> recs(num_rays);
    for (size_t s = 0; s < num_rays; ++s)
        recs[s].id = rays[s].id;

    // See what they hit (SLOW)
    for (size_t s = 0; s < num_rays; ++s) {
        BatchedHitRecord &bhr = recs[s];
        bhr.hit_something = r_ctx.scene->hit(rng, rays[s].ray, static_cast<real>(0.001), Infinity, bhr.rec);
    }

    // Sort the hit records based on if they hit something or not
    sort(recs.begin(), recs.end(), [](const BatchedHitRecord &a, const BatchedHitRecord &b) {
        return a.hit_something == b.hit_something;
    });

    // Count how many scatered rays we;ll need
    size_t num_hit = count_if(recs.begin(), recs.end(), [](const BatchedHitRecord &rec) { return rec.hit_something; });
    vector<BatchedRay> scattered_rays(num_hit);
    vector<BatchedVec3> attenuations(num_hit);
    vector<BatchedVec3> emittions(num_hit);

    Vec3<

    for (size_t s = 0; s < num_rays; ++s) {
        // Note that the hit reocords are now out of order (in the `id` sense)
        BatchedHitRecord &bhr = recs[s];
        const size_t id = bhr.id;

        if (bhr.hit_something) {
            // DO emittion and scatter check
            // Mark which rays scattered, then recurse on them
            scattered_rays[id].id = id;
            attenuations[id].id = id;

        } else {
            // It hit the backound, match the hit record tht that of the return vec
            vecs[id].id = id;
            vecs[id].v = r_ctx.background;
        }
    }

    // Hit something...
    Ray scattered;
    Vec3 attenuation;
    const Vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    const bool rays_scattered = rec.mat_ptr->scatter(rng, r, rec, attenuation, scattered);

    // If hit a an emmsive material (they don't scatter rays), then only emit that one's light
    if (!rays_scattered)
        return emitted;

    // else, it must have been non-emissive,
    return emitted + (attenuation * ray_color(r_ctx, rng, scattered, depth - 1));

}
*/


// This was an attempt to see if an interative ray color function could be more performant
//   than the recursive one
// TODO do a little measureing to see if it does help
Vec3 ray_color_iterative(const RenderContext &r_ctx, RandomGenerator &rng, const Ray &initial_ray, const uint32_t depth) NOEXCEPT {
    vector<Vec3> clrs;  // TODO reserve a specific size
    uint32_t d = depth;
    bool hit_something = false;
    Ray ray = initial_ray;

    while (d != 0) {
        // Hit the end of our depth
        if (d == 0) {
            clrs.push_back(Vec3(0));
            break;   // Break out early
        }

        HitRecord rec{};

        // See if we hit something
        hit_something = r_ctx.scene->hit(rng, ray, static_cast<rreal>(0.001), Infinity, rec);
        if (!hit_something) {
            // Didn't hit a thing, pop on the bacgrkound and exit early
            clrs.push_back(r_ctx.background);
            break;   // Break out earily
        }

        // Hit something...
        Ray scattered;
        Vec3 attenuation;
        const Vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        const bool rays_scattered = rec.mat_ptr->scatter(rng, ray, rec, attenuation, scattered);

        // If hit a an emmsive material (they don't scatter rays), then only emit that one's light
        if (!rays_scattered) {
            // Push on the emittied color and break out early
            clrs.push_back(emitted);
            break;
        }

        // It Hit another emissive material, with the scattered ray, go deeper
        clrs.push_back(emitted + attenuation);
        ray = scattered;
        d--;
    }

    // Hit nothing...
    if (clrs.empty())
        return Vec3(0);

    // Now summate the accumulated colors (starting in reverse)
    Vec3 final_clr = *clrs.rbegin();
    auto riter = clrs.rbegin() + 1;
    while (riter != clrs.rend()) {
        final_clr = final_clr * (*riter);
        riter++;
    }

    return final_clr;
}

/*
// This is an experiment to compute a color for a pixel, with a batch of rays, in an iterative manor
//   It hopes to explain hot pathing and data locality
Vec3 ray_color_iterative_batched(const RenderContext &r_ctx, RandomGenerator &rng, const std::vector<Ray> &rays, const uint32_t depth) {
    const size_t num_rays = rays.size();
    const real r_num_rays = static_cast<real>(num_rays);

    // Preallocate to the maximum possible number of rays
    vector<vector<Vec3>> clrs(static_cast<size_t>(depth));
    for (auto &vv : clrs)
        vv.reserve(num_rays);

    uint32_t d = depth;
    while (d != 0) {
        vector<Vec3> &
    }


    Vec3 clr(0);
    clr.x /= r_num_rays;
    clr.y /= r_num_rays;
    clr.z /= r_num_rays;
//    pixel.a /= num_rays;

    return Vec3(0);
}
*/


ColorRGBA _pixel_color(const RenderContext &r_ctx, RandomGenerator &rng, const uint32_t samples_per_pixel, const rreal x, const rreal y, const uint16_t max_ray_depth) NOEXCEPT {
    // The result
    ColorRGBA pixel;

    const auto height_max = static_cast<uint16_t>(r_ctx.height - 1);
    const auto r_spp = static_cast<rreal>(samples_per_pixel);

#ifdef USE_BOOK_COMPUTE_PIXEL_COLOR
    for (uint32_t s = 0; s < samples_per_pixel; ++s) {
        const Ray r = r_ctx.camera->get_ray(
            rng,
                 (x + rng.get_real()) / static_cast<rreal>(r_ctx.width - 1),
            1 - ((y + rng.get_real()) / static_cast<rreal>(height_max))
        );
        const Vec3 clr = ray_color(r_ctx, rng, r, max_ray_depth);

        pixel.r += clr.x;
        pixel.g += clr.y;
        pixel.b += clr.z;
    }
#else
    // Compared to the book, this method instead involves pooling the randoms offsets, rays,
    // and colours and then doing the averaging after all the ray colors have been computed
    // Pool random offsets
    vector<array<rreal, 2>> uv_pairs(samples_per_pixel);
    for (auto &uv : uv_pairs) {
        uv[0] =      (x + rng.get_real()) / static_cast<rreal>(r_ctx.width - 1);
        uv[1] = 1 - ((y + rng.get_real()) / static_cast<rreal>(height_max));
    }

    // Pool some rays here
    vector<Ray> rays(samples_per_pixel);
    for (uint32_t s = 0; s < samples_per_pixel; ++s) {
        rays[s] = r_ctx.camera->get_ray(rng, uv_pairs[s][0], uv_pairs[s][1]);
    }

    vector<Vec3> clrs(samples_per_pixel);
    for (uint32_t s = 0; s < samples_per_pixel; ++s) {
        clrs[s] = ray_color(r_ctx, rng, rays[s], max_ray_depth);
    }

    // Put the pixel in their accumulator
    for (const Vec3 &c : clrs) {
        pixel.r += c.x;
        pixel.g += c.y;
        pixel.b += c.z;
    }
#endif  // USE_BOOK_COMPUTE_PIXEL_COLOR

    // Average all the samples
    pixel.r /= r_spp;
    pixel.g /= r_spp;
    pixel.b /= r_spp;
//    pixel.a /= r_spp;

    // Gamma correction (g=2.0)
    pixel.r = util::sqrt(pixel.r);
    pixel.g = util::sqrt(pixel.g);
    pixel.b = util::sqrt(pixel.b);
    pixel.a = util::sqrt(pixel.a);

    return pixel;
}
