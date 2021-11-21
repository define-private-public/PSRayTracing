#include "render.h"
#include <chrono>
#include <sstream>
#include "Interfaces/ICamera.h"
#include "Scenes/All.h"
#include "RenderThread.h"
#include "RenderOutput.h"

using namespace std;


namespace psrt {


// Some global state management functions.  I'm not too happy about doing this, but I
// want to keep things simple
atomic<float> g_progress(0);
atomic_bool g_render_in_progress(false);
atomic_bool g_stop_render_requested(false);



future<Render> do_render(const RenderJob &job)
{
    // If we're uing the book perlin and have the deep copy per thread on, we're going to crash
    // (due to seg fault IIRC). Instead we're going to prevent that here (by erroring early) and
    // explaining the issue.
    #ifdef USE_BOOK_PERLIN
        constexpr bool using_book_perlin = true;
    #else
        constexpr bool using_book_perlin = false;
    #endif

    if (job.deep_copy_per_thread && using_book_perlin)
    {
        stringstream msg;
        msg << "[!! WARNING]  This was compiled to use the Book's method for Perlin noise." << endl
             << "              having `copy-per-thread` enabled may cause segfaults when using" << endl
             << "              Perlin noise.  Please either disable this flag or re-compile with" << endl
             << "              `USE_BOOK_PERLIN=OFF`" << endl;
        throw runtime_error(msg.str());
    }

    // Check top see if the scene is available or not
    const bool scene_not_found = (find(AllSceneIds.cbegin(), AllSceneIds.cend(), job.scene_id) == AllSceneIds.cend());
    if (scene_not_found)
    {
        // Did they request a scene that wasn't available?
        stringstream msg;
        msg << "Wasn't able to find scene with Id: `" << job.scene_id << "`";
        throw invalid_argument(msg.str());
    }

    // If there's already a render in progress, don't let another one start
    if (g_render_in_progress.load())
        throw runtime_error("Render already in progress; cannot start a second one");

    // Reset some state
    g_progress.store(0);
    g_stop_render_requested.store(false);
    g_render_in_progress.store(true);

    // Setup the task to render
    future<Render> task = async(launch::async, [job]
    {
        // Setup the render description
        const RenderOutput render_desc(job.render_width, job.render_height);
        const rreal aspect_ratio = static_cast<rreal>(render_desc.width) / static_cast<rreal>(render_desc.height);

        // Prep the render
        const SceneCameraPair scp = AvailableScenes.at(job.scene_id)(aspect_ratio);
        const SceneDescriptor scene_desc = scp.scene;
        const shared_ptr<ICamera> cam = scene_desc.cameras[scp.camera_number];

        // Setup the context
        RenderContext r_ctx;
        r_ctx.scene = scene_desc.scene;
        r_ctx.camera = cam;
        r_ctx.background = scene_desc.background;
        r_ctx.width = render_desc.width;
        r_ctx.height = render_desc.height;
        r_ctx.deep_copy_per_thread = job.deep_copy_per_thread;

        // Create the thread pool (and renderer)
        RenderThreadPool pool(r_ctx, job.num_threads);
        pool.setup_render(job.seed_str, job.samples_per_pixel, job.max_depth);

        // Do the render!
        const auto startTime = chrono::system_clock::now();
        bool user_reqeusted_stop_render = false;
        pool.start_render();
        do
        {
            // Accumulate the progress
            const auto progress_percent = static_cast<float>(pool.num_pixels_completed()) / static_cast<float>(pool.total_pixel_count());
            g_progress.store(progress_percent);

            // Check if were were supposed to stop or not
            user_reqeusted_stop_render = g_stop_render_requested.load();

            // Sleep for 1/20th of a second (we dont' need to update the state variables all the time, give more time to the CPU to render)
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        while (!pool.render_completed() && !user_reqeusted_stop_render);

        // Cleanly terminate threads
        pool.shutdown_and_wait();

        const auto endTime = chrono::system_clock::now();

        // Store the result
        Render result;
        result.image_data.assign(render_desc.total_byte_size, 0);       // Allocate the size for the destination buffer

        // Grab the render and save it (only if it completed succesfully)
        if (!user_reqeusted_stop_render)
        {
            g_progress.store(1.0f);
            pool.retreive_render(render_desc, result.image_data);
        }

        result.render_width = render_desc.width;
        result.render_height = render_desc.height;
        result.num_channels = render_desc.num_channels;
        result.render_time_in_seconds = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()) / 1000.0f;
        result.render_time_in_nanoseconds = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

        g_render_in_progress.store(false);

        return result;
    });

    return task;
}


// Turn on the flag to kill the render
void stop_active_render()
    { g_stop_render_requested.store(true); }


float render_progress()
    { return g_progress.load(); }



bool render_in_progress()
    { return g_render_in_progress.load(); }


vector<string> all_scene_ids()
{
    const vector<string> vec(AllSceneIds.cbegin(), AllSceneIds.cend());
    return vec;
}

int num_concurrent_threads_supported()
    { return static_cast<int>(std::thread::hardware_concurrency()); }


}   // psrt::
