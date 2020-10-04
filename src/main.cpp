#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#include "third_party/stb_image_write.h"
#include "Util.h"
#include "Util/ProgressBar.h"
#include "Interfaces/ICamera.h"
#include "Scenes/All.h"
#include "RenderThread.h"
#include "RenderOutput.h"

using namespace std;
namespace po = boost::program_options;


int main(int argc, char *argv[]) {
    // Build the argument parser
    po::variables_map args{};
    po::options_description argsDesc{"Options"};
    argsDesc.add_options()
        ("help,h",                                                                      "Help screen (this message)")
        ("list-scenes",                                                                 "List all of the available scenes to render")
        ("scene",             po::value<string>()->default_value("book2::final_scene"), "Scene to render")
        ("size,s",            po::value<string>()->default_value("960x540"),            "Render size")
        ("num-samples,n",     po::value<int32_t>()->default_value(10),                  "Samples per pixel")
        ("num-threads,j",     po::value<int>()->default_value(1),                       "How many threads to render with")
        ("depth,d",           po::value<int16_t>()->default_value(50),                  "Maximum ray bounce depth")
        ("random-seed,r",     po::value<string>()->default_value("ASDF"),               "Seed string for the RNG")
        ("output-filename,o", po::value<string>()->default_value("render.png"),         "Filename to save render to (PNG only)")
        ("no-progress-bar",   po::bool_switch()->default_value(false),                  "Don't show the progress bar when rendering");

    try {
        po::store(po::parse_command_line(argc, argv, argsDesc), args);
        po::notify(args);
    } catch (const po::error &err) {
        cerr << err.what() << endl;
        return 1;
    }

    // Show help message? (Exit early)
    if (args.count("help")) {
        cout << argsDesc << endl;
        return 0;
    }

    // Show available scense? (Exit Early)
    if (args.count("list-scenes")) {
        cout << "Available Scenes:" << endl;
        for (const string &s_id: AllSceneIds)
            cout << "  " << s_id << endl;
        return 0;
    }

    // Parse the size argument
    const auto size_str = args["size"].as<string>();
    uint16_t render_width = 0, render_height = 0;
    if (!util::try_parse_size_string(size_str, render_width, render_height)) {
        cerr << "Failed to parse size from string `" << size_str << "`" << endl;
        return 1;
    }

    // Parse the output name, make sure there is at least one character
    const auto output_filename = args["output-filename"].as<string>();
    if (output_filename.empty()) {
        cerr << "Output filename must have at least one character" << endl;
        return 1;
    }

    // Setup the render description (and destination buffer)
    const RenderOutput render_desc(render_width, render_height);
    vector<uint8_t> image_data(render_desc.total_byte_size, 0);

    // Get configuration options from command line, `max(1, n)` calls are here to prevent sneaky users from inputing negative values
    const auto scene_id = args["scene"].as<string>();
    const auto samples_per_pixel = static_cast<uint32_t>(std::max(static_cast<int32_t>(1), args["num-samples"].as<int32_t>()));
    const auto max_depth = static_cast<uint16_t>(std::max(static_cast<int16_t>(1), args["depth"].as<int16_t>()));
    const auto num_threads = std::max(args["num-threads"].as<int>(), 1);
    const auto show_progress = !args["no-progress-bar"].as<bool>();
    const auto seed_str = args["random-seed"].as<string>();

    // Other vars
    const rreal aspect_ratio = static_cast<rreal>(render_desc.width) / static_cast<rreal>(render_desc.height);

    // Setup what's going to be renderded
    SceneCameraPair scp;
    try {
        scp = AvailableScenes.at(scene_id)(aspect_ratio);
    } catch (const out_of_range &err) {
        // Did they request a scene that wasn't available?
        cerr << "Wasn't able to find scene with Id: `" << scene_id << "`" << endl;
        return 1;
    }

    // Print some info
    cout << "Scene: " << scene_id << endl;
    cout << "Render size: " << render_desc.width << "x" << render_desc.height << endl;
    cout << "Samples per pixel: " << samples_per_pixel << endl;
    cout << "Max number of ray bounces: " << max_depth << endl;
    cout << "Number of render threads: " << num_threads << endl;
    cout << "Saving to: " << output_filename << endl;
    cout << "Seed: `" << seed_str << "`" << endl;

    // Prep the render
    const SceneDescriptor scene_desc = scp.scene;
    const shared_ptr<ICamera> cam = scene_desc.cameras[scp.camera_number];

    // Setup the context
    RenderContext r_ctx;
    r_ctx.scene = scene_desc.scene;
    r_ctx.camera = cam;
    r_ctx.background = scene_desc.background;
    r_ctx.width = render_desc.width;
    r_ctx.height = render_desc.height;

    // Create the thread pool (and renderer)
    RenderThreadPool pool(r_ctx, static_cast<uint16_t>(num_threads));
    pool.setup_render(seed_str, samples_per_pixel, max_depth);

    // Progress bar
    util::ProgressBar progBar(pool.total_pixel_count(), 60, "Rendering:");
    if (show_progress)
        progBar.start();

    // Do the render!
    const auto startTime = chrono::system_clock::now();
    pool.start_render();
    do {
        if (show_progress) {
            progBar.set_to(pool.num_pixels_completed());
            progBar.display(cout);
        }
        // Sleep for 1/10th of a second
        this_thread::sleep_for(chrono::milliseconds(100));
    } while (!pool.render_completed());

    // Cleanly terminate threads
    pool.shutdown_and_wait();

    const auto endTime = chrono::system_clock::now();
    if (show_progress) {
        progBar.set_to(pool.total_pixel_count());       // Doesn't always hit the 100%, so we need to do this
        progBar.done(cout);
    }

    // Print some info
    const rreal renderTime = rreal(chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) / 1000.0f;
    cout << "Render took " << renderTime << " seconds" << endl;

    // Grab the render and save it
    pool.retreive_render(render_desc, image_data);
    stbi_write_png(
        output_filename.c_str(),
        static_cast<int>(render_desc.width),
        static_cast<int>(render_desc.height),
        render_desc.num_channels,
        image_data.data(),
        0
    );

    return 0;
}
