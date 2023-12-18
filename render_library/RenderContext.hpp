#pragma once

#include <memory>
#include "Vec3.hpp"
#include "RenderMethod.hpp"
class IHittable;
class ICamera;


// The general context of what needs to be rendered
//   Treat this as a read-only structure
struct RenderContext
{
    // Data
    std::shared_ptr<IHittable> scene = nullptr;             // Objects to render
    std::shared_ptr<IHittable> lights = nullptr;            // Light sources in the scene
    std::shared_ptr<ICamera> camera = nullptr;              // Camera to render with
    Vec3 background = Vec3(0);                              // Background of scene, affects the illumination
    uint16_t width = 1;                                     // Render image width
    uint16_t height = 1;                                    // Render image height
    RenderMethod render_method = RenderMethod::Books1And2;  // The style of rendering to use (e.g. book 3 uses PDFs)
    bool deep_copy_per_thread = true;                       // If there should be a of the scene & camera per each thread
};
