#include "Scenes/Fun.h"
#include <memory>
#include "RandomGenerator.h"
#include "Materials/Lambertian.h"
#include "Materials/SurfaceNormal.h"
#include "Materials/Metal.h"
#include "Materials/Dielectric.h"
#include "Materials/DiffuseLight.h"
#include "Textures/CheckerTexture.h"
#include "Objects/Sphere.h"
#include "Objects/HittableList.h"
#include "Objects/Rectangles.h"
#include "Objects/Box.h"
#include "Objects/BVHNode.h"
#include "Objects/BVHNode_MorePerformant.h"
#include "Objects/Transform/RotateY.h"
#include "Objects/Transform/Translate.h"
#include "Cameras/Camera.h"

using namespace std;


// Are we to use the book's BVH node, or our more performant one?
#if WITH_BOOK_BVH_NODE
    using BVHNode_Implementation = BVHNode;
#else
    using BVHNode_Implementation = BVHNode_MorePerformant;
#endif


constexpr rreal glass_refractive_index = 1.5;


namespace Scenes { namespace Fun {

SceneDescriptor three_spheres(const rreal aspect_ratio) {
    const Vec3 ground_yellow(static_cast<rreal>(0.8), static_cast<rreal>(0.8), static_cast<rreal>(0));
    const Vec3 gold(static_cast<rreal>(0.8), static_cast<rreal>(0.6), static_cast<rreal>(0.2));
    const Vec3 blue(static_cast<rreal>(0.1), static_cast<rreal>(0.2), 2);
    const auto ground_mat = make_shared<Lambertian>(ground_yellow);
    const auto blue_mat   = make_shared<Lambertian>(blue);
    const auto left_mat   = make_shared<Dielectric>(glass_refractive_index);
    const auto center_mat = make_shared<SurfaceNormal>();
    const auto right_mat  = make_shared<Metal>(gold, 1);

    const auto tiny_y = static_cast<rreal>(0.005);
    HittableList world;
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1),  0.5f,  left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1), -0.35f, left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1),  0.3f,  left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1), -0.2f,  left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1),  0.15f, left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,  tiny_y, -1),  0.1f,  blue_mat));
    world.add(make_shared<Sphere>(Vec3( 0,  static_cast<rreal>(0.15) , -1),   .5f,  center_mat));
    world.add(make_shared<Sphere>(Vec3( 1,  tiny_y, -1),  0.5f,  right_mat));
    world.add(make_shared<Sphere>(Vec3( 0, -100.5, -1),   100,  ground_mat));   // The ground (Big boi)

    const Vec3 look_from(0, 0.25, 3.25);
    const Vec3 look_at(0, 0, -1);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();

    const auto cam = make_shared<Camera>(look_from, look_at, v_up, 25, aspect_ratio, 0.5, dist_to_focus);

    RandomGenerator rng(DefaultRNGSeed);
    SceneDescriptor sd;
    sd.background = sky_blue;
    sd.scene = make_shared<BVHNode_Implementation>(rng, world, 0, 0);
    sd.cameras.push_back(cam);

    return sd;
}

SceneDescriptor whitted_1980(const rreal aspect_ratio) {
    const rreal glass_refractive_index = 1.5;

    // Colors need to be squared since they are square rooted at the end of the render
    const Vec3 sky(
        static_cast<rreal>(pow(static_cast<rreal>(0.45), 2)),
        static_cast<rreal>(pow(static_cast<rreal>(0.63), 2)),
        static_cast<rreal>(pow(static_cast<rreal>(0.87), 2))
    );
    const Vec3 red(
        static_cast<rreal>(pow(static_cast<rreal>(0.85), 2)),
        static_cast<rreal>(pow(static_cast<rreal>(0.35), 2)),
        static_cast<rreal>(pow(static_cast<rreal>(0.25), 2))
    );
    const Vec3 yellow(
        pow(static_cast<rreal>(1), 2),
        pow(static_cast<rreal>(1), 2),
        pow(static_cast<rreal>(0), 2)
    );

    auto checker_pattern = make_shared<CheckerTexture>(red, yellow);
    checker_pattern->x_frequency = checker_pattern->y_frequency = checker_pattern->z_frequency = 18;

    const auto floor_mat = make_shared<Lambertian>(checker_pattern);
    const auto sun_light = make_shared<DiffuseLight>(Vec3(35));
    const auto glass_mat = make_shared<Dielectric>(glass_refractive_index);
    const auto mirror_mat = make_shared<Metal>(Vec3(static_cast<rreal>(0.8), 1, 1), 0.05);

    const rreal r = static_cast<rreal>(0.24);

    HittableList world;

    // Floor
    world.add(make_shared<XZRect>(static_cast<rreal>(-0.8), static_cast<rreal>(1.7), -0.5, static_cast<rreal>(4.2), 0, floor_mat));

    // Glass sphere
    const Vec3 glass_sphere_center(static_cast<rreal>(1.08), 0.5, static_cast<rreal>(3.85));
    world.add(make_shared<Sphere>(glass_sphere_center, r, glass_mat));
    world.add(make_shared<Sphere>(glass_sphere_center, -r * 0.95, glass_mat));

    // Mirror sphere
    const Vec3 mirror_sphere_center(0.75, static_cast<rreal>(0.34), static_cast<rreal>(3.45));
    world.add(make_shared<Sphere>(mirror_sphere_center, r, mirror_mat));

    // Sun
    world.add(make_shared<Sphere>(Vec3(1.5, 7, 5), 1, sun_light));

    // The camera
    const Vec3 look_from(1, 0.5, 5);
    const Vec3 look_at(1, 0, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();

    const auto cam = make_shared<Camera>(look_from, look_at, v_up, 35, aspect_ratio, 0, dist_to_focus);

    SceneDescriptor sd;
    sd.background = sky;
    sd.scene = make_shared<HittableList>(world);
    sd.cameras.push_back(cam);

    return sd;
}

SceneDescriptor cornell_glass_boxes(const rreal aspect_ratio) {
    constexpr rreal light_intensity = 5;

    const auto red   = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.65), static_cast<rreal>(0.05), static_cast<rreal>(0.05)));
    const auto white = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.73)));
    const auto green = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.12), static_cast<rreal>(0.45), static_cast<rreal>(0.15)));
    const auto light = make_shared<DiffuseLight>(Vec3(static_cast<rreal>(light_intensity)));
    const auto glass = make_shared<Dielectric>(glass_refractive_index);

    // The walls (and light)
    HittableList world;
    world.add(make_shared<YZRect>(  0, 555,   0, 555, 555, green));     // Left
    world.add(make_shared<YZRect>(  0, 555,   0, 555,   0, red));       // Right
    world.add(make_shared<XZRect>(150, 410, 150, 400,   5, light));
    world.add(make_shared<XZRect>(  0, 555,   0, 555,   0, white));     // Floor
    world.add(make_shared<XZRect>(  0, 555,   0, 555, 555, white));     // Roof
    world.add(make_shared<XYRect>(  0, 555,   0, 555, 555, white));     // Back

    constexpr int cube_depth = 5;           // Makes NxN cubes
    constexpr rreal box_size = 50;
    constexpr rreal box_spacing = 50;
    constexpr rreal box_offset = 45;

    // Make the NxN cubs
    for (int z = 0; z < cube_depth; z++) {
        for (int y = 0; y < cube_depth; y++) {
            for (int x = 0; x < cube_depth; x++) {
                // Figure out a location and orientation
                const Vec3 loc(
                    box_offset + ((box_size + box_spacing) * x),
                    box_offset + ((box_size + box_spacing) * y),
                    -42.5      + ((box_size + box_spacing) * z)
                );
                const rreal rot = (23 * x) + (13 * y) + (3 * z);

                // Add it in
                shared_ptr<IHittable> box = make_shared<Box>(Vec3(0), Vec3(box_size), glass);
                box = make_shared<RotateY>(box, rot);
                box = make_shared<Translate>(box, loc);
                world.add(box);
            }
        }
    }

    RandomGenerator rng(DefaultRNGSeed);
    SceneDescriptor sd{};
    sd.scene = make_shared<BVHNode_Implementation>(rng, world, 0, 0);
    sd.background = 0.8 * sky_blue;

    const Vec3 look_from(278, 278, -800);
    const Vec3 look_at(278, 278, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();
    const rreal aperture = 0;
    sd.cameras.push_back(make_shared<Camera>(look_from, look_at, v_up, 40, aspect_ratio, aperture, dist_to_focus));

    return sd;
}

}}  // Scenes::Fun
