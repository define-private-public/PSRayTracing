#pragma once

#include "Common.hpp"
#include "SceneDescriptor.hpp"

// Scenes from Book 1 (Ray Tracing in one Weekend) of the Peter Shirley Raytracing minioooks


namespace Scenes {
    namespace Book1 {
        SceneDescriptor surface_normal_sphere(const rreal aspect_ratio);
        SceneDescriptor grey_sphere(const rreal aspect_ratio);
        SceneDescriptor shiny_metal_spheres(const rreal aspect_ratio);
        SceneDescriptor fuzzy_metal_spheres(const rreal aspect_ratio);
        SceneDescriptor two_glass_one_metal_spheres(const rreal aspect_ratio);
        SceneDescriptor glass_blue_metal_spheres(const rreal aspect_ratio);
        SceneDescriptor blue_red_spheres(const rreal aspect_ratio);

        // This one has multiple cameras.  They are as follows:
        //   0: standard view
        //   1: far view,
        //   2: close view
        //   3: close & fuzzy view
        SceneDescriptor hollow_glass_blue_metal_spheres(const rreal aspect_ratio);

        SceneDescriptor final_scene(const rreal aspect_ratio);
    }
}
