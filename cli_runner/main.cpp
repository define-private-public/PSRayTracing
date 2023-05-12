#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include "Misc.h"
#include "ProgressBar.h"
#include "../third_party/cxxopts.hpp"
#include "../third_party/stb_image_write.h"
#include "../render_library/render.h"

using namespace std;
namespace co = cxxopts;
using namespace psrt;


int main(int argc, char *argv[]) {
    // Build the (machine specific) help message for number of threads`
    const int num_recommended_threads = num_concurrent_threads_supported();
    stringstream num_threads_msg;
    num_threads_msg << "How many threads to render with; " << num_recommended_threads << " recomended for this device";

    // Build the argument parser
    co::Options options("PSRayTracing", "An optimized implementation of Peter Shirley's Ray Tracing minibook series");
    options.add_options()
        ("h,help",              "Help screen (this message)")
        ("list-scenes",         "List all of the available scenes to render")
        ("scene",               "Scene to render",                                              co::value<string>()->default_value("book2::final_scene"))
        ("s,size",              "Render size",                                                  co::value<string>()->default_value("960x540"))
        ("n,num-samples",       "Samples per pixel",                                            co::value<int32_t>()->default_value("10"))
        ("j,num-threads",       num_threads_msg.str(),                                          co::value<int>()->default_value("1"))
        ("d,depth",             "Maximum ray bounce depth",                                     co::value<int16_t>()->default_value("50"))
        ("r,random-seed",       "Seed string for the RNG",                                      co::value<string>()->default_value("ASDF"))
        ("o,output-filename",   "Filename to save render to (PNG only)",                        co::value<string>()->default_value("render.png"))
        ("no-progress-bar",     "Don't show the progress bar when rendering",                   co::value<bool>()->default_value("false"))
        ("no-copy-per-thread",  "Don't make a copy of scene per thread",                        co::value<bool>()->default_value("false"))
        ("testing-mode",        "Run in testing mode; only outputs how long render time took",  co::value<bool>()->default_value("false"))
    ;

    // And then parse them
    const co::ParseResult args = options.parse(argc, argv);

    // Show help message? (Exit early)
    if (args.count("help"))
    {
        cout << options.help() << endl;
        return 0;
    }

    // Show available scense? (Exit Early)
    if (args.count("list-scenes"))
    {
        cout << "Available Scenes:" << endl;
        for (const string &s_id: all_scene_ids())
            cout << "  " << s_id << endl;
        return 0;
    }

    // Parse the size argument
    const auto size_str = args["size"].as<string>();
    uint16_t render_width = 0, render_height = 0;
    if (!try_parse_size_string(size_str, render_width, render_height))
    {
        cerr << "Failed to parse size from string `" << size_str << "`" << endl;
        return 1;
    }

    // Parse the output name, make sure there is at least one character
    const auto output_filename = args["output-filename"].as<string>();
    if (output_filename.empty())
    {
        cerr << "Output filename must have at least one character" << endl;
        return 1;
    }

    // Get configuration options from command line, `max(1, n)` calls are here to prevent sneaky users from inputing negative values
    RenderJob job;
    job.render_width         = render_width;
    job.render_height        = render_height;
    job.scene_id             = args["scene"].as<string>();
    job.samples_per_pixel    = static_cast<uint32_t>(std::max(static_cast<int32_t>(1), args["num-samples"].as<int32_t>()));
    job.max_depth            = static_cast<uint16_t>(std::max(static_cast<int16_t>(1), args["depth"].as<int16_t>()));
    job.num_threads          = static_cast<uint16_t>(std::max(args["num-threads"].as<int>(), 1));
    job.seed_str             = args["random-seed"].as<string>();
    job.deep_copy_per_thread = !args["no-copy-per-thread"].as<bool>();

    const auto run_in_testing_mode = args["testing-mode"].as<bool>();
    auto show_progress = !args["no-progress-bar"].as<bool>();

    // If we're in test mode, there's going to be no progress bar
    if (run_in_testing_mode)
        show_progress = false;

    // Print some info
    if (!run_in_testing_mode)
    {
        cout << "Scene: " << job.scene_id << endl;
        cout << "Render size: " << job.render_width << "x" << job.render_height << endl;
        cout << "Samples per pixel: " << job.samples_per_pixel << endl;
        cout << "Max number of ray bounces: " << job.max_depth << endl;
        cout << "Number of render threads: " << job.num_threads << endl;
        cout << "  Copy per thread: " << (job.deep_copy_per_thread ? "on" : "off")<< endl;
        cout << "Saving to: " << output_filename << endl;
        cout << "Seed: `" << job.seed_str << "`" << endl;
    }

    // Progress bar
    ProgressBar prog_bar(100, 50, "Rendering:");
    if (show_progress)
        prog_bar.start();

    // Do the render!
    future<Render> render_task = do_render(job);
    float progress = 0.0f;
    do
    {
        progress = render_progress();

        if (show_progress)
        {
            prog_bar.set_to(static_cast<uint64_t>(100.0f * progress));
            prog_bar.display(cout);
        }

        // Sleep for 1/10th of a second
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    while (progress < 1.0f);

    if (show_progress)
        prog_bar.done(cout);

    // Retrive the result of the render
    render_task.wait();
    Render result = render_task.get();

    // Print some info
    if (run_in_testing_mode)
    {
        // For machines, we show nanoseconds (as integers)
        cout << result.render_time_in_nanoseconds.count() << " ns" << endl;
    }
    else
    {
        // for humans, we show seconds (with decimals)
        cout << "Render took " << result.render_time_in_seconds << " seconds" << endl;
    }

    // Grab the render and save it
    stbi_write_png(
        output_filename.c_str(),
        static_cast<int>(result.render_width),
        static_cast<int>(result.render_height),
        result.num_channels,
        result.image_data.data(),
        0
    );

    return 0;
}
