#include "Book1.hpp"
#include <random>
#include "Cameras/Camera.hpp"
#include "RandomGenerator.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/SurfaceNormal.hpp"
#include "Materials/Metal.hpp"
#include "Materials/Dielectric.hpp"
#include "Objects/Sphere.hpp"
#include "Objects/HittableList.hpp"
#include "Util.hpp"

using namespace std;


//==== Scene setup helpers ====
// (avoid that duplicate code)

namespace Scenes { namespace Book1 {

const Vec3 _ground_yellow(static_cast<rreal>(0.8), static_cast<rreal>(0.8), static_cast<rreal>(0));
const Vec3 _gold(static_cast<rreal>(0.8), static_cast<rreal>(0.6), static_cast<rreal>(0.2));
const Vec3 _blue(static_cast<rreal>(0.1), static_cast<rreal>(0.2), 0.5);
const rreal _rightmost_fuzz = 1;
const rreal _glass_refractive_index = 1.5;
const auto _zero_aperature = 0;     // Everything is in focus due to super tiny aperature


// Get's a common "everything is in focus camera" that's used for most of the early scenes
shared_ptr<ICamera>_center_looking_focused_camera(const rreal aspect_ratio) {
    const Vec3 look_from(0, 0.25, 2.5);
    const Vec3 look_at(0, 0, -1);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();

    return make_shared<Camera>(look_from, look_at, v_up, 25, aspect_ratio, _zero_aperature, dist_to_focus);
}

shared_ptr<IHittable> _single_sphere(const shared_ptr<IMaterial> &sphere_mat, const shared_ptr<IMaterial> &ground_mat) {
    HittableList world;
    world.add(make_shared<Sphere>(Vec3( 0,      0, -1),  0.5f, sphere_mat));   // Little boi
    world.add(make_shared<Sphere>(Vec3( 0, -100.5, -1),   100, ground_mat));   // The ground (Big boi)

    return make_shared<HittableList>(world);
}

shared_ptr<IHittable> _tripple_spheres(
    const shared_ptr<IMaterial> &left_mat,
    const shared_ptr<IMaterial> &center_mat,
    const shared_ptr<IMaterial> &right_mat,
    const shared_ptr<IMaterial> &ground_mat
) {
    HittableList world;
    world.add(make_shared<Sphere>(Vec3(-1,      0, -1), 0.5, left_mat));
    world.add(make_shared<Sphere>(Vec3( 0,      0, -1), 0.5, center_mat));
    world.add(make_shared<Sphere>(Vec3( 1,      0, -1), 0.5, right_mat));
    world.add(make_shared<Sphere>(Vec3( 0, -100.5, -1), 100, ground_mat));   // The ground (Big boi)

    return make_shared<HittableList>(world);
}

shared_ptr<IHittable> _metal_spheres(const rreal leftmost_fuzz, const rreal rightmost_fuzz) {
    const Vec3 light_grey(static_cast<rreal>(0.8));
    const Vec3 soft_red(static_cast<rreal>(0.7), static_cast<rreal>(0.3), static_cast<rreal>(0.3));

    const auto ground_mat = make_shared<Lambertian>(_ground_yellow);
    const auto left_mat   = make_shared<Metal>(light_grey, leftmost_fuzz);
    const auto center_mat = make_shared<Lambertian>(soft_red);
    const auto right_mat  = make_shared<Metal>(_gold, rightmost_fuzz);

    return _tripple_spheres(left_mat, center_mat, right_mat, ground_mat);
}


//====  Actual Scene code ====
SceneDescriptor surface_normal_sphere(const rreal aspect_ratio) {
    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));

    // Create the materials
    auto little_mat = make_shared<SurfaceNormal>();
    auto big_mat = make_shared<SurfaceNormal>();

    sd.scene = _single_sphere(little_mat, big_mat);

    return sd;
}

SceneDescriptor grey_sphere(const rreal aspect_ratio) {
    auto grey_mat = make_shared<Lambertian>(Vec3(0.5));

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));
    sd.scene = _single_sphere(grey_mat, grey_mat);

    return sd;
}

SceneDescriptor shiny_metal_spheres(const rreal aspect_ratio) {
    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));
    sd.scene = _metal_spheres(0, 0);

    return sd;
}

SceneDescriptor fuzzy_metal_spheres(const rreal aspect_ratio) {
    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));
    sd.scene = _metal_spheres(static_cast<rreal>(0.3), _rightmost_fuzz);

    return sd;
}

SceneDescriptor two_glass_one_metal_spheres(const rreal aspect_ratio) {
    const auto ground_mat = make_shared<Lambertian>(_ground_yellow);
    const auto glass_mat = make_shared<Dielectric>(_glass_refractive_index);
    const auto right_mat  = make_shared<Metal>(_gold, _rightmost_fuzz);

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));
    sd.scene = _tripple_spheres(glass_mat, glass_mat, right_mat, ground_mat);

    return sd;
}

SceneDescriptor glass_blue_metal_spheres(const rreal aspect_ratio) {
    const auto ground_mat = make_shared<Lambertian>(_ground_yellow);
    const auto left_mat   = make_shared<Dielectric>(_glass_refractive_index);
    const auto center_mat = make_shared<Lambertian>(_blue);
    const auto right_mat  = make_shared<Metal>(_gold, 0);

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));
    sd.scene = _tripple_spheres(left_mat, center_mat, right_mat, ground_mat);

    return sd;
}

SceneDescriptor blue_red_spheres(const rreal aspect_ratio) {
    // TODO there is a bug if the origin=(0, 0, -1) and the lookat=(0, 0, 0)
    // Setup a focused wide-angle camera  (that `90` part)
    const Vec3 look_from(0, 0, static_cast<rreal>(0.0005));
    const Vec3 look_at(0, 0, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();
    const auto cam = make_shared<Camera>(look_from, look_at, v_up, 90, aspect_ratio, _zero_aperature, dist_to_focus);

    // Materials
    const auto left_mat = make_shared<Lambertian>(Vec3(0, 0, 1));       // Hard blue
    const auto right_mat = make_shared<Lambertian>(Vec3(1, 0, 0));      // Hard red

    // Spheres
    const rreal R = std::cos(Pi / 4);
    auto world = make_shared<HittableList>();
    world->add(make_shared<Sphere>(Vec3(-R, 0, -1), R, left_mat));
    world->add(make_shared<Sphere>(Vec3( R, 0, -1), R, right_mat));

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.scene = world;
    sd.cameras.push_back(cam);

    return sd;
}

SceneDescriptor hollow_glass_blue_metal_spheres(const rreal aspect_ratio) {
    const auto ground_mat = make_shared<Lambertian>(_ground_yellow);
    const auto left_mat   = make_shared<Dielectric>(_glass_refractive_index);
    const auto center_mat = make_shared<Lambertian>(_blue);
    const auto right_mat  = make_shared<Metal>(_gold, 0);

    HittableList world;
    world.add(make_shared<Sphere>(Vec3(-1,      0, -1),    0.5f, left_mat));
    world.add(make_shared<Sphere>(Vec3(-1,      0, -1),  -0.45f, left_mat));
    world.add(make_shared<Sphere>(Vec3( 0,      0, -1),     .5f, center_mat));
    world.add(make_shared<Sphere>(Vec3( 1,      0, -1),    0.5f, right_mat));
    world.add(make_shared<Sphere>(Vec3( 0, -100.5, -1),     100, ground_mat));   // The ground (Big boi)

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.scene = make_shared<HittableList>(world);

    // For the distant/close cameras (in focus)
    const Vec3 look_from(-2, 2, 1);
    const Vec3 look_at(0, 0, -1);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();

    sd.cameras.push_back(_center_looking_focused_camera(aspect_ratio));                                                       // Standard front on view
    sd.cameras.push_back(make_shared<Camera>(look_from, look_at, v_up, 90, aspect_ratio, _zero_aperature, dist_to_focus));    // Distant view
    sd.cameras.push_back(make_shared<Camera>(look_from, look_at, v_up, 20, aspect_ratio, _zero_aperature, dist_to_focus));    // Close view

    // For the not so in focus
    const Vec3 look_from_fuzzy(3, 3, 2);
    const rreal dist_to_focus_fuzzy = (look_from_fuzzy - look_at).length();
    sd.cameras.push_back(make_shared<Camera>(look_from_fuzzy, look_at, v_up, 20, aspect_ratio, 2, dist_to_focus_fuzzy));

    return sd;
}

// The final scene with all the spheres
SceneDescriptor final_scene(const rreal aspect_ratio) {
    SceneDescriptor sd{};
    sd.background = sky_blue;

    // Camera
    const Vec3 look_from(13, 2, 3);
    const Vec3 look_at(0, 0, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = 10;
    const auto aperture = static_cast<rreal>(0.1);
    sd.cameras.push_back(make_shared<Camera>(look_from, look_at, v_up, 20, aspect_ratio, aperture, dist_to_focus));

    // World
    HittableList world;

    // The ground
    auto ground_material = make_shared<Lambertian>(Vec3(0.5));
    world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    // The RNG that's used to setup the scene
    // We don't use `rreal` here since we want the generated scene to be the same regardless of `rreal`'s precision
    _GeneralizedRandomGenerator<uniform_real_distribution, double, mt19937> rng(DefaultRNGSeed);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const rreal choose_mat = rng.get_real();
            const Vec3 center(
                static_cast<rreal>(a) + (static_cast<rreal>(0.9) * rng.get_real()),
                static_cast<rreal>(0.2),
                static_cast<rreal>(b) + (static_cast<rreal>(0.9) * rng.get_real())
            );
            const rreal len = (center - Vec3(4, static_cast<rreal>(0.2), 0)).length();

            if (len > 0.8) {
                shared_ptr<IMaterial> sphere_material = nullptr;

                if (choose_mat < 0.8) {
                    // Diffuse
                    const Vec3 albedo = rng.get_vec3() * rng.get_vec3();
                    sphere_material = make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // Metal
                    const Vec3 albedo = rng.get_vec3(0.5, 1);
                    const rreal fuzz = rng.get_real(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                } else {
                    // Glass
                    sphere_material = make_shared<Dielectric>(1.5);
                }

                world.add(make_shared<Sphere>(center, 0.2, sphere_material));
            }
        }
    }

    // The focal points
    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1, material1));

    auto material2 = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.4), static_cast<rreal>(0.2), static_cast<rreal>(0.1)));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1, material2));

    auto material3 = make_shared<Metal>(Vec3(static_cast<rreal>(0.7), static_cast<rreal>(0.6), static_cast<rreal>(0.5)), 0);
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1, material3));

    sd.scene = make_shared<HittableList>(world);
    return sd;
}

}}  // Scenes::Book1
