#pragma once

#include <memory>
#include <vector>
#include "Vec3.h"

class IHittable;
class ICamera;


// A Scene descriptor is a simple object that stores the defnition of
// how a scene should look.  It contains objects as well as camera
// configurations
struct SceneDescriptor {
    // Data
    std::shared_ptr<IHittable> scene;
    std::vector<std::shared_ptr<ICamera>> cameras;
    Vec3 background = Vec3(0);
};

// That sky blue background that was used for all things in the first book and a half
const Vec3 sky_blue(static_cast<rreal>(0.7), static_cast<rreal>(0.8), 1);
