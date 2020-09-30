#pragma once

#include "Common.h"
#include "SceneDescriptor.h"
#include "PerlinReal.h"
#include "Textures/NoiseTexture.h"

// Scenes from Book 2 (Ray Tracing the Next Week) of the Peter Shirley Raytracing minioooks


namespace Scenes {
    namespace Book2 {
        // These have blurry (motion) objects in them
        SceneDescriptor bouncing_spheres(const rreal aspect_ratio, bool use_bvh_node=false);
        SceneDescriptor bouncing_spheres_checkered_floor(const rreal aspect_ratio, bool use_bvh_node=false);

        SceneDescriptor two_checkered_spheres(const rreal aspect_ratio);

        // There are many different versions of the two perlin spheres
        SceneDescriptor perlin_sphere(const rreal aspect_ratio, const PerlinReal::InterpolationType interpType);     // Uses the Perlin with the `reals`
        SceneDescriptor perlin_sphere(const rreal aspect_ratio, const NoiseTexture::NoiseStyle noise_style);         // Uses the Perlin with the `Vec3`

        SceneDescriptor earth(const rreal aspect_ratio);
        SceneDescriptor simple_light(const rreal aspect_ratio, const bool overhead_sphere_light=false);

        // There are a few variations on the cornel box we have, so we need to use an enum to differentaite
        enum class CornellBoxConfiguration {
            Empty = 0,
            TwoBoxes,
            TwoRotatedBoxes,
            TwoSmokeBoxes,
        };
        SceneDescriptor cornell_box(const rreal aspect_ratio, const CornellBoxConfiguration config);

        SceneDescriptor final_scene(const rreal aspect_ratio);
    }
}

