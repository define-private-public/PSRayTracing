#include "Book3.h"
#include <cmath>
#include <random>
#include "Cameras/Camera.h"
#include "Cameras/MotionBlurCamera.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectric.h"
#include "Materials/DiffuseLight.h"
#include "Objects/Sphere.h"
#include "Objects/HittableList.h"
#include "Objects/Rectangles.h"
#include "Objects/BVHNode.h"
#include "Objects/BVHNode_MorePerformant.h"
#include "Objects/Box.h"
#include "Objects/Transform/Translate.h"
#include "Objects/Transform/RotateY.h"
#include "Objects/Transform/FlipFace.h"

using namespace std;


// Are we to use the book's BVH node, or our more performant one?
#if WITH_BOOK_BVH_NODE
    using BVHNode_Implementation = BVHNode;
#else
    using BVHNode_Implementation = BVHNode_MorePerformant;
#endif


namespace Scenes { namespace Book3 {

//==== Scene setup helpers ====
// (avoid that duplicate code)


SceneDescriptor cornell_box(const rreal aspect_ratio, const CornellBoxConfiguration config) {
    const rreal light_intensity = 15;

    const auto red = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.65), static_cast<rreal>(0.05), static_cast<rreal>(0.05)));
    const auto white = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.73)));
    const auto green = make_shared<Lambertian>(Vec3(static_cast<rreal>(0.12), static_cast<rreal>(0.45), static_cast<rreal>(0.15)));
    const auto light = make_shared<DiffuseLight>(Vec3(static_cast<rreal>(light_intensity)));
    const auto aluminium = make_shared<Metal>(Vec3(0.8, 0.85, 0.88), 0);

    // The light source(s)
    HittableList lights;
    const auto ceiling_light = make_shared<XZRect>(213, 343, 227, 332, 554, light);
    lights.add(ceiling_light);

    // The walls (and light)
    HittableList world;
    world.add(make_shared<YZRect>(  0, 555,   0, 555, 555, green));     // Left
    world.add(make_shared<YZRect>(  0, 555,   0, 555,   0, red));       // Right
    world.add(make_shared<FlipFace>(ceiling_light));
    world.add(make_shared<XZRect>(  0, 555,   0, 555,   0, white));     // Floor
    world.add(make_shared<XZRect>(  0, 555,   0, 555, 555, white));     // Roof
    world.add(make_shared<XYRect>(  0, 555,   0, 555, 555, white));     // Back

    // default Material of the left item should be white (else, a mirror)
    shared_ptr<IMaterial> left_mat = white;
    if ((config == CornellBoxConfiguration::TwoBoxesOneMirror) || (config == CornellBoxConfiguration::OneMirrorBoxOneGlassSphere))
        left_mat = aluminium;

    // Left item is always a box
    shared_ptr<IHittable> left_item = make_shared<Box>(Vec3(0), Vec3(165, 330, 165), left_mat);
    left_item = make_shared<RotateY>(left_item, 15);
    left_item = make_shared<Translate>(left_item, Vec3(265, 0, 295));

    // Right item is either a box or a sphere
    shared_ptr<IHittable> right_item;
    if ((config == CornellBoxConfiguration::TwoBoxes) || (config == CornellBoxConfiguration::TwoBoxesOneMirror)) {
        // default right item should be a box
        right_item = make_shared<Box>(Vec3(0), Vec3(165), white);
        right_item = make_shared<RotateY>(right_item, -18);
        right_item = make_shared<Translate>(right_item, Vec3(130, 0, 65));
    } else {
        // Else, it 's a (glass) sphere
        const auto glass = make_shared<Dielectric>(1.5);
        const Vec3 loc(190, 90, 190);
        const rreal r = 90;
        right_item = make_shared<Sphere>(loc, r, glass);

        // This also requires an extra light
        lights.add(make_shared<Sphere>(loc, r, light));
    }

    world.add(left_item);
    world.add(right_item);

    SceneDescriptor sd{};
    sd.render_method = RenderMethod::Book3;
    sd.scene = make_shared<HittableList>(world);
    sd.lights = make_shared<HittableList>(lights);

    const Vec3 look_from(278, 278, -800);
    const Vec3 look_at(278, 278, 0);
    const Vec3 v_up(0, 1, 0);
    const rreal dist_to_focus = 10;//(look_from - look_at).length();
    const rreal aperture = 0;
    sd.cameras.push_back(make_shared<MotionBlurCamera>(look_from, look_at, v_up, 40, aspect_ratio, aperture, dist_to_focus, 0, 1));

    return sd;
}

}}  // Scenes::Book3
