#include "Book2.h"
#include <cmath>
#include <random>
#include "Cameras/Camera.h"
#include "Cameras/MotionBlurCamera.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectric.h"
#include "Materials/DiffuseLight.h"
#include "Objects/Sphere.h"
#include "Objects/MovingSphere.h"
#include "Objects/HittableList.h"
#include "Objects/Rectangles.h"
#include "Objects/BVHNode.h"
#include "Objects/BVHNode_MorePerformant.h"
#include "Objects/Box.h"
#include "Objects/Transform/Translate.h"
#include "Objects/Transform/RotateY.h"
#include "Objects/ConstantMedium.h"
#include "Textures/CheckerTexture.h"
#include "Textures/NoiseTexture.h"
#include "Textures/ImageTexture.h"
#include "Perlin.h"
#include "PerlinReal.h"
#include "RandomGenerator.h"
#include "../assets/earthmap.jpg.h"

using namespace std;


// Are we to use the book's BVH node, or our more performant one?
#if WITH_BOOK_BVH_NODE
    using BVHNode_Implementation = BVHNode;
#else
    using BVHNode_Implementation = BVHNode_MorePerformant;
#endif


namespace Scenes { namespace Book2 {

//==== Scene setup helpers ====
// (avoid that duplicate code)

const auto _green_white_checkered_mat = make_shared<Lambertian>(make_shared<CheckerTexture>(
    Vec3(static_cast<rreal>(0.2), static_cast<rreal>(0.3), static_cast<rreal>(0.1)),
    Vec3(static_cast<rreal>(0.9))
));

shared_ptr<MotionBlurCamera> _common_camera(const rreal aspect_ratio, const rreal aperture) {
    const Vec3 look_from(13, 2, 3);
    const Vec3 look_at(0, 0, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = 10;
    return make_shared<MotionBlurCamera>(look_from, look_at, v_up, 20, aspect_ratio, aperture, dist_to_focus, 0, 1);
}

SceneDescriptor _bouncing_spheres_common(const rreal aspect_ratio, const shared_ptr<IMaterial> &ground_mat, const bool use_bvh_node) {
    SceneDescriptor sd{};
    sd.background = sky_blue;

    // Camera
    sd.cameras.push_back(_common_camera(aspect_ratio, static_cast<rreal>(0.1)));

    // World
    HittableList world;

    // The ground
    world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_mat));

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
                    const Vec3 center2 = center + Vec3(0, rng.get_real(0, 0.5), 0);
                    world.add(make_shared<MovingSphere>(center, center2, 0, 1, static_cast<rreal>(0.2), sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal
                    const Vec3 albedo = rng.get_vec3(0.5, 1);
                    const rreal fuzz = rng.get_real(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
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

    if (use_bvh_node) {
        RandomGenerator bvhRNG(DefaultRNGSeed);
        sd.scene = make_shared<BVHNode>(bvhRNG, world, 0, 1);
    } else
        sd.scene = make_shared<HittableList>(world);

    return sd;
}

SceneDescriptor _perlin_sphere_common(const rreal aspect_ratio, const shared_ptr<NoiseTexture> &noise_tex) {
    // The world (and objects)
    const auto perlin_mat = make_shared<Lambertian>(noise_tex);
    auto objs = make_shared<HittableList>();
    objs->add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, perlin_mat));
    objs->add(make_shared<Sphere>(Vec3(0,  2, 0), 2, perlin_mat));

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_common_camera(aspect_ratio, 0.0));
    sd.scene = objs;

    return sd;
}


//====  Actual Scene code ====
SceneDescriptor bouncing_spheres(const rreal aspect_ratio, bool use_bvh_node) {
    return _bouncing_spheres_common(aspect_ratio, make_shared<Lambertian>(Vec3(0.5)), use_bvh_node);
}

SceneDescriptor bouncing_spheres_checkered_floor(const rreal aspect_ratio, bool use_bvh_node) {
    return _bouncing_spheres_common(aspect_ratio, _green_white_checkered_mat, use_bvh_node);
}

SceneDescriptor two_checkered_spheres(const rreal aspect_ratio) {
    SceneDescriptor sd{};
    sd.background = sky_blue;

    HittableList world;
    world.add(make_shared<Sphere>(Vec3(0, -10, 0), 10, _green_white_checkered_mat));
    world.add(make_shared<Sphere>(Vec3(0,  10, 0), 10, _green_white_checkered_mat));

    shared_ptr<MotionBlurCamera> cam = _common_camera(aspect_ratio, 0.0);

    sd.cameras.push_back(cam);
    sd.scene = make_shared<HittableList>(world);

    return sd;
}

SceneDescriptor perlin_sphere(const rreal aspect_ratio, const PerlinReal::InterpolationType interp_type) {
    // There are kind of a few perlin noise scenes, this one uses the `PerlinReal` varaiant
    rreal noise_scale = 1;
    if (interp_type == PerlinReal::InterpolationType::TriLinearHermiteCubic)
        noise_scale = 4;                    // Noise was bumped up in scale later on

    RandomGenerator perlin_rng(DefaultRNGSeed);

    const auto noise_tex = make_shared<NoiseTexture>(make_shared<PerlinReal>(perlin_rng, interp_type), noise_scale);
    return _perlin_sphere_common(aspect_ratio, noise_tex);
}

SceneDescriptor perlin_sphere(const rreal aspect_ratio, const NoiseTexture::NoiseStyle noise_style) {
    // There are kind of a few perlin noise scenes, this one uses the `PerlinVec` varaiant, which is the final one of the book

    RandomGenerator perlin_rng(DefaultRNGSeed);

    auto noise_tex = make_shared<NoiseTexture>(make_shared<Perlin>(perlin_rng), 4);
    noise_tex->style = noise_style;
    return _perlin_sphere_common(aspect_ratio, noise_tex);
}

SceneDescriptor earth(const rreal aspect_ratio) {
    // The world (quite literlaly)
//    const shared_ptr<ImageTexture> earth_texture = ImageTexture::load_from_file("assets/earthmap.jpg");
    const shared_ptr<ImageTexture> earth_texture = ImageTexture::load_from_memory_buffer(earthmap_jpg, earthmap_jpg_len);
    const auto earth_surface = make_shared<Lambertian>(earth_texture);

    SceneDescriptor sd{};
    sd.background = sky_blue;
    sd.cameras.push_back(_common_camera(aspect_ratio, 0.0));
    sd.scene = make_shared<Sphere>(Vec3(0), 2, earth_surface);

    return sd;
}

SceneDescriptor simple_light(const rreal aspect_ratio, const bool overhead_sphere_light) {
    RandomGenerator perlin_rng(DefaultRNGSeed);

    // The world (and objects)
    const auto noise_tex = make_shared<NoiseTexture>(make_shared<Perlin>(perlin_rng), 4);
    const auto perlin_mat = make_shared<Lambertian>(noise_tex);
    HittableList world;
    world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, perlin_mat));
    world.add(make_shared<Sphere>(Vec3(0,  2, 0), 2, perlin_mat));

    // Light(s)
    const auto diff_light = make_shared<DiffuseLight>(Vec3(4));
    world.add(make_shared<XYRect>(3, 5, 1, 3, -2, diff_light));

    if (overhead_sphere_light)
        world.add(make_shared<Sphere>(Vec3(0,  7, 0), 2, diff_light));

    SceneDescriptor sd{};   // Used default black screen
    sd.scene = make_shared<HittableList>(world);

    const Vec3 look_from(26, 3, 6);
    const Vec3 look_at(0, 2, 0);
    const Vec3 vup(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();
    const rreal aperture = 0;
    sd.cameras.push_back(make_shared<MotionBlurCamera>(look_from, look_at, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1));

    return sd;
}

SceneDescriptor cornell_box(const rreal aspect_ratio, const CornellBoxConfiguration config) {
    const rreal light_intensity = (config == CornellBoxConfiguration::TwoSmokeBoxes) ? 7 : 15;

    const auto red = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.65), static_cast<rreal>(0.05), static_cast<rreal>(0.05)));
    const auto white = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.73)));
    const auto green = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.12), static_cast<rreal>(0.45), static_cast<rreal>(0.15)));
    const auto light = make_shared<DiffuseLight>(Vec3(static_cast<rreal>(light_intensity)));

    // The walls (and light)
    HittableList world;
    world.add(make_shared<YZRect>(  0, 555,   0, 555, 555, green));     // Left
    world.add(make_shared<YZRect>(  0, 555,   0, 555,   0, red));       // Right
    if (config == CornellBoxConfiguration::TwoSmokeBoxes)                // Light
        world.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
    else
        world.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    world.add(make_shared<XZRect>(  0, 555,   0, 555,   0, white));     // Floor
    world.add(make_shared<XZRect>(  0, 555,   0, 555, 555, white));     // Roof
    world.add(make_shared<XYRect>(  0, 555,   0, 555, 555, white));     // Back

    if (config == CornellBoxConfiguration::TwoBoxes) {
        world.add(make_shared<Box>(Vec3(130, 0,  64), Vec3(295, 165, 240), white));     // Left box
        world.add(make_shared<Box>(Vec3(265, 0, 295), Vec3(430, 330, 460), white));     // Right box
    } else if ((config == CornellBoxConfiguration::TwoRotatedBoxes) || (config == CornellBoxConfiguration::TwoSmokeBoxes)) {
        shared_ptr<IHittable> left_box = make_shared<Box>(Vec3(0), Vec3(165, 330, 165), white);
        left_box = make_shared<RotateY>(left_box, 15);
        left_box = make_shared<Translate>(left_box, Vec3(265, 0, 295));

        shared_ptr<IHittable> right_box = make_shared<Box>(Vec3(0), Vec3(165), white);
        right_box = make_shared<RotateY>(right_box, -18);
        right_box = make_shared<Translate>(right_box, Vec3(130, 0, 65));

        if (config == CornellBoxConfiguration::TwoSmokeBoxes) {
            world.add(make_shared<ConstantMedium>(left_box, static_cast<rreal>(0.01), Vec3(0)));
            world.add(make_shared<ConstantMedium>(right_box, static_cast<rreal>(0.01), Vec3(1)));
        } else {
            world.add(left_box);
            world.add(right_box);
        }
    }

    SceneDescriptor sd{};
    sd.scene = make_shared<HittableList>(world);

    const Vec3 look_from(278, 278, -800);
    const Vec3 look_at(278, 278, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = 10;//(look_from - look_at).length();
    const rreal aperture = 0;
    sd.cameras.push_back(make_shared<MotionBlurCamera>(look_from, look_at, v_up, 40, aspect_ratio, aperture, dist_to_focus, 0, 1));

    return sd;
}

SceneDescriptor final_scene(const rreal aspect_ratio) {
    // The RNG that's used to setup the scene
    // We don't use `rreal` here since we want the generated scene to be the same regardless of `rreal`'s precision
    _GeneralizedRandomGenerator<uniform_real_distribution, double, mt19937> scene_rng(DefaultRNGSeed);

    // But we also need an RNG for other construction (e.g. BVHNode)
    RandomGenerator req_rng(DefaultRNGSeed);

    HittableList objects;

    // Make the boxes that lie on the ground
    HittableList boxes1;
    const auto ground = make_shared<Lambertian>(Vec3(
        static_cast<rreal>(0.48),
        static_cast<rreal>(0.83),
        static_cast<rreal>(0.53)
    ));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            const rreal w = 100;
            const rreal x0 = -1000 + (static_cast<rreal>(i) * w);
            const rreal z0 = -1000 + (static_cast<rreal>(j) * w);
            const rreal y0 = 0;
            const rreal x1 = x0 + w;
            const rreal y1 = scene_rng.get_real(1, 101);
            const rreal z1 = z0 + w;

            boxes1.add(make_shared<Box>(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground));
        }
    }

    objects.add(make_shared<BVHNode_Implementation>(req_rng, boxes1, 0, 1));

    // Illumination source
    const auto light = make_shared<DiffuseLight>(Vec3(7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    // Moving sphere
    const Vec3 center1(400, 400, 200);
    const Vec3 center2 = center1 + Vec3(30, 0, 0);
    const auto moving_sphere_mat = make_shared<Lambertian>(Vec3(
        static_cast<rreal>(0.7),
        static_cast<rreal>(0.3),
        static_cast<rreal>(0.1)
    ));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_mat));

    // Solid glass sphere
    objects.add(make_shared<Sphere>(Vec3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));

    // fuzzy metal sphere
    objects.add(make_shared<Sphere>(Vec3(0, 150, 145), 50, make_shared<Metal>(Vec3(
        static_cast<rreal>(0.8),
        static_cast<rreal>(0.8),
        static_cast<rreal>(0.9)
    ), 10)));

    // glazzy sphere with blue smoke
    auto boundary = make_shared<Sphere>(Vec3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(
        boundary,
        static_cast<rreal>(0.2),
        Vec3(
            static_cast<rreal>(0.2),
            static_cast<rreal>(0.4),
            static_cast<rreal>(0.9)
        )
    ));

    // General fog around the scene
    boundary = make_shared<Sphere>(Vec3(0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, static_cast<rreal>(0.0001), Vec3(1)));

    // Earth
//    const auto emat = make_shared<Lambertian>(ImageTexture::load_from_file("assets/earthmap.jpg"));
    const auto emat = make_shared<Lambertian>(ImageTexture::load_from_memory_buffer(earthmap_jpg, earthmap_jpg_len));
    objects.add(make_shared<Sphere>(Vec3(400, 200, 400), 100, emat));

    // Perlin sphere
    const auto perlin_tex = make_shared<NoiseTexture>(make_shared<Perlin>(req_rng), static_cast<rreal>(0.1));
    perlin_tex->style = NoiseTexture::NoiseStyle::Book2FinalScene;
    objects.add(make_shared<Sphere>(Vec3(220, 280, 300), 80, make_shared<Lambertian>(perlin_tex)));

    // Many little spheres
    HittableList boxes2;
    const auto white = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.73)));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
        boxes2.add(make_shared<Sphere>(scene_rng.get_vec3(0, 165), 10, white));

    objects.add(make_shared<Translate>(
        make_shared<RotateY>(
            make_shared<BVHNode_Implementation>(req_rng, boxes2, 0, 1),
            15
        ),
        Vec3(-100, 270, 395)
    ));

    // Camera
    const Vec3 look_from(478, 278, -600);
    const Vec3 look_at(278, 278, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = (look_from - look_at).length();
    const rreal aperture = 0;

    SceneDescriptor sd{};
    sd.scene = make_shared<BVHNode_Implementation>(req_rng, objects, 0, 1);
    sd.cameras.push_back(make_shared<MotionBlurCamera>(look_from, look_at, v_up, 40, aspect_ratio, aperture, dist_to_focus, 0, 1));

    return sd;
}

}}  // Scenes::Book2
