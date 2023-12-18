#pragma once

#include <memory>
#include <vector>
#include "Vec3.hpp"
#include "RenderMethod.hpp"

class IHittable;
class ICamera;


// A Scene descriptor is a simple object that stores the defnition of
// how a scene should look.  It contains objects as well as camera
// configurations
struct SceneDescriptor {
    // Data
    std::shared_ptr<IHittable> scene;
    std::shared_ptr<IHittable> lights;                      ///< This could be a list of lights, or a singular one; note that this is meant for use with book3 style scenes/renders
    std::vector<std::shared_ptr<ICamera>> cameras;
    Vec3 background = Vec3(0);
    RenderMethod render_method = RenderMethod::Books1And2;  ///< With style of rendering to use; e.g. Book 3 uses PDFs
};

// That sky blue background that was used for all things in the first book and a half
const Vec3 sky_blue(static_cast<rreal>(0.7), static_cast<rreal>(0.8), 1);
