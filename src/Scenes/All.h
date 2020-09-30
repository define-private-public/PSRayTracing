#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include "Scenes/Book1.h"
#include "Scenes/Book2.h"
#include "Scenes/Fun.h"


// A complete configuration for a scene to render, and which camera to use
struct SceneCameraPair {
    SceneDescriptor scene;
    size_t camera_number = 0;         // By default, all scenes must have one camera, (and most only have one)
};


// Possible scene ids (in order)
const std::string Book1_SurfaceNormalSphere                    = "book1::surface_normal_sphere";
const std::string Book1_GreySphere                             = "book1::grey_sphere";
const std::string Book1_ShinyMetalSphere                       = "book1::shiny_metal_sphere";
const std::string Book1_FuzzyMetalSphere                       = "book1::fuzzy_metal_sphere";
const std::string Book1_TwoGlassOneMetalSpheres                = "book1::two_glass_one_metal_spheres";
const std::string Book1_GlassBlueMetalSpheres                  = "book1::glass_blue_metal_spheres";
const std::string Book1_RedBlueSpheres                         = "book1::red_blue_spheres";
const std::string Book1_HollowGlassBlueMetalSpheres            = "book1::glass_blue_metal_spheres";
const std::string Book1_HollowGlassBlueMetalSpheres_Far        = "book1::glass_blue_metal_spheres::far_view";
const std::string Book1_HollowGlassBlueMetalSpheres_Close      = "book1::glass_blue_metal_spheres::close_view";
const std::string Book1_HollowGlassBlueMetalSpheres_CloseFuzzy = "book1::glass_blue_metal_spheres::close_fuzzy_view";
const std::string Book1_FinalScene                             = "book1::final_scene";

const std::string Book2_BouncingSpheres                        = "book2::bouncing_spheres";
const std::string Book2_BouncingSpheres_WithBVH                = "book2::bouncing_spheres::with_bvh";
const std::string Book2_BouncingSpheresCheckeredFloor          = "book2::bouncing_spheres_checkered_floor";
const std::string Book2_BouncingSpheresCheckeredFloor_WithBVH  = "book2::bouncing_spheres_checkered_floor::with_bvh";
const std::string Book2_TwoCheckeredSpheres                    = "book2::two_checkered_spheres";
const std::string Book2_PerlinSphere                           = "book2::perlin_sphere";
const std::string Book2_PerlinSphere_Trilinear                 = "book2::perlin_sphere::trilinear";
const std::string Book2_PerlinSphere_TrilinearHermiteCubic     = "book2::perlin_sphere::trilinear_hermite_cubic";
const std::string Book2_PerlinSphere_NormalNoise               = "book2::perlin_sphere::normal_noise";
const std::string Book2_PerlinSphere_TurbulentNoise            = "book2::perlin_sphere::turbulent_noise";
const std::string Book2_PerlinSphere_MarbleNoise               = "book2::perlin_sphere::marble_noise";
const std::string Book2_Earth                                  = "book2::earth";
const std::string Book2_SimpleLight                            = "book2::simple_light";
const std::string Book2_SimpleLight_WithOverheadLight          = "book2::simple_light::with_overhead_light";
const std::string Book2_CornellBox_Empty                       = "book2::cornell_box::empty";
const std::string Book2_CornellBox_TwoBoxes                    = "book2::cornell_box::two_boxes";
const std::string Book2_CornellBox_TwoRotatedBoxes             = "book2::cornell_box::two_rotated_boxes";
const std::string Book2_CornellBox_TwoSmokeBoxes               = "book2::cornell_box::two_smoke_boxes";
const std::string Book2_FinalScene                             = "book2::final_scene";

const std::string Fun_ThreeSpheres                             = "fun::three_spheres";
const std::string Fun_Whitted1980                              = "fun::whitted_1980";

// All of the scenes (in order)
const std::array<std::string, 33> AllSceneIds = {
    Book1_SurfaceNormalSphere,
    Book1_GreySphere,
    Book1_ShinyMetalSphere,
    Book1_FuzzyMetalSphere,
    Book1_TwoGlassOneMetalSpheres,
    Book1_GlassBlueMetalSpheres,
    Book1_RedBlueSpheres,
    Book1_HollowGlassBlueMetalSpheres,
    Book1_HollowGlassBlueMetalSpheres_Far,
    Book1_HollowGlassBlueMetalSpheres_Close,
    Book1_HollowGlassBlueMetalSpheres_CloseFuzzy,
    Book1_FinalScene,
    Book2_BouncingSpheres,
    Book2_BouncingSpheres_WithBVH,
    Book2_BouncingSpheresCheckeredFloor,
    Book2_BouncingSpheresCheckeredFloor_WithBVH,
    Book2_TwoCheckeredSpheres,
    Book2_PerlinSphere,
    Book2_PerlinSphere_Trilinear,
    Book2_PerlinSphere_TrilinearHermiteCubic,
    Book2_PerlinSphere_NormalNoise,
    Book2_PerlinSphere_TurbulentNoise,
    Book2_PerlinSphere_MarbleNoise ,
    Book2_Earth,
    Book2_SimpleLight,
    Book2_SimpleLight_WithOverheadLight,
    Book2_CornellBox_Empty,
    Book2_CornellBox_TwoBoxes,
    Book2_CornellBox_TwoRotatedBoxes,
    Book2_CornellBox_TwoSmokeBoxes,
    Book2_FinalScene,
    Fun_ThreeSpheres,
    Fun_Whitted1980,
};


// A map for a scene Id to a function that spits out that scene configuration
const std::unordered_map<std::string, std::function<SceneCameraPair(rreal)>> AvailableScenes {
    /*== Book 1 ==*/
    {Book1_SurfaceNormalSphere, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::surface_normal_sphere(aspect_ratio);
        return scp;
    }},
    {Book1_GreySphere, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::grey_sphere(aspect_ratio);
        return scp;
    }},
    {Book1_ShinyMetalSphere, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::shiny_metal_spheres(aspect_ratio);
        return scp;
    }},
    {Book1_FuzzyMetalSphere, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::fuzzy_metal_spheres(aspect_ratio);
        return scp;
    }},
    {Book1_TwoGlassOneMetalSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::two_glass_one_metal_spheres(aspect_ratio);
        return scp;
    }},
    {Book1_GlassBlueMetalSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::glass_blue_metal_spheres(aspect_ratio);
        return scp;
    }},
    {Book1_RedBlueSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::blue_red_spheres(aspect_ratio);
        return scp;
    }},
    {Book1_HollowGlassBlueMetalSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::hollow_glass_blue_metal_spheres(aspect_ratio);
        scp.camera_number = 0;
        return scp;
    }},
    {Book1_HollowGlassBlueMetalSpheres_Far, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::hollow_glass_blue_metal_spheres(aspect_ratio);
        scp.camera_number = 1;
        return scp;
    }},
    {Book1_HollowGlassBlueMetalSpheres_Close, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::hollow_glass_blue_metal_spheres(aspect_ratio);
        scp.camera_number = 2;
        return scp;
    }},
    {Book1_HollowGlassBlueMetalSpheres_CloseFuzzy, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::hollow_glass_blue_metal_spheres(aspect_ratio);
        scp.camera_number = 3;
        return scp;
    }},
    {Book1_FinalScene, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book1::final_scene(aspect_ratio);
        return scp;
    }},
    {Book2_BouncingSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::bouncing_spheres(aspect_ratio);
        return scp;
    }},
    {Book2_BouncingSpheres_WithBVH, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::bouncing_spheres(aspect_ratio, true);
        return scp;
    }},
    {Book2_BouncingSpheresCheckeredFloor, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::bouncing_spheres_checkered_floor(aspect_ratio);
        return scp;
    }},
    {Book2_BouncingSpheresCheckeredFloor_WithBVH, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::bouncing_spheres_checkered_floor(aspect_ratio, true);
        return scp;
    }},
    {Book2_TwoCheckeredSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::two_checkered_spheres(aspect_ratio);
        return scp;
    }},
    {Book2_PerlinSphere, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, PerlinReal::InterpolationType::None);
        return scp;
    }},
    {Book2_PerlinSphere_Trilinear, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, PerlinReal::InterpolationType::TriLinear);
        return scp;
    }},
    {Book2_PerlinSphere_TrilinearHermiteCubic, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, PerlinReal::InterpolationType::TriLinearHermiteCubic);
        return scp;
    }},
    {Book2_PerlinSphere_NormalNoise, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, NoiseTexture::NoiseStyle::Normal);
        return scp;
    }},
    {Book2_PerlinSphere_TurbulentNoise, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, NoiseTexture::NoiseStyle::Turbulent);
        return scp;
    }},
    {Book2_PerlinSphere_MarbleNoise, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::perlin_sphere(aspect_ratio, NoiseTexture::NoiseStyle::Marble);
        return scp;
    }},
    {Book2_Earth, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::earth(aspect_ratio);
        return scp;
    }},
    {Book2_SimpleLight, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::simple_light(aspect_ratio);
        return scp;
    }},
    {Book2_SimpleLight_WithOverheadLight, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::simple_light(aspect_ratio, true);
        return scp;
    }},
    {Book2_CornellBox_Empty, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::cornell_box(aspect_ratio, Scenes::Book2::CornellBoxConfiguration::Empty);
        return scp;
    }},
    {Book2_CornellBox_TwoBoxes, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::cornell_box(aspect_ratio, Scenes::Book2::CornellBoxConfiguration::TwoBoxes);
        return scp;
    }},
    {Book2_CornellBox_TwoRotatedBoxes, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::cornell_box(aspect_ratio, Scenes::Book2::CornellBoxConfiguration::TwoRotatedBoxes);
        return scp;
    }},
    {Book2_CornellBox_TwoSmokeBoxes, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::cornell_box(aspect_ratio, Scenes::Book2::CornellBoxConfiguration::TwoSmokeBoxes);
        return scp;
    }},
    {Book2_FinalScene, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Book2::final_scene(aspect_ratio);
        return scp;
    }},
    {Fun_ThreeSpheres, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Fun::three_spheres(aspect_ratio);
        return scp;
    }},
    {Fun_Whitted1980, [](const rreal aspect_ratio) {
        SceneCameraPair scp;
        scp.scene = Scenes::Fun::whitted_1980(aspect_ratio);
        return scp;
    }},
};
