cmake_minimum_required(VERSION 3.10)

project(PSRayTracing_StaticLibrary)

set(CMAKE_CXX_STANDARD 17)

# Get the source
set(sources
  render.hpp
  render.cpp

  AABB.hpp
  AABB.cpp
  ColorRGBA.hpp
  ColorRGBA.cpp
  Common.hpp
  HitRecord.hpp
  HitRecord.cpp
  ONB.hpp
  ONB.cpp
  PDFVariant.hpp
  PDFVariant.cpp
  Perlin.hpp
  Perlin.cpp
  PerlinReal.hpp
  PerlinReal.cpp
  RandomGenerator.hpp
  RandomGenerator.cpp
  Ray.hpp
  Ray.cpp
  RenderContext.hpp
  RenderContext.cpp
  RenderMethod.hpp
  RenderMethod.cpp
  RenderOutput.hpp
  RenderOutput.cpp
  RenderThread.hpp
  RenderThread.cpp
  RenderThreadPool.hpp
  RenderThreadPool.cpp
  SceneDescriptor.hpp
  ScatterRecord.hpp
  ScatterRecord.cpp
  Util.hpp
  Util.cpp
  Vec3.hpp
  Vec3.cpp

  Cameras/Camera.hpp
  Cameras/Camera.cpp
  Cameras/MotionBlurCamera.hpp
  Cameras/MotionBlurCamera.cpp

  Interfaces/ICamera.hpp
  Interfaces/IDeepCopyable.hpp
  Interfaces/IHittable.hpp
  Interfaces/IMaterial.hpp
  Interfaces/IPDF.hpp
  Interfaces/ITexture.hpp

  Materials/Dielectric.hpp
  Materials/Dielectric.cpp
  Materials/DiffuseLight.hpp
  Materials/DiffuseLight.cpp
  Materials/Isotropic.hpp
  Materials/Isotropic.cpp
  Materials/Lambertian.hpp
  Materials/Lambertian.cpp
  Materials/Metal.hpp
  Materials/Metal.cpp
  Materials/SurfaceNormal.hpp
  Materials/SurfaceNormal.cpp

  Objects/Box.hpp
  Objects/Box.cpp
  Objects/BVHNode.hpp
  Objects/BVHNode.cpp
  Objects/BVHNode_MorePerformant.hpp
  Objects/BVHNode_MorePerformant.cpp
  Objects/ConstantMedium.hpp
  Objects/ConstantMedium.cpp
  Objects/HittableList.hpp
  Objects/HittableList.cpp
  Objects/MovingSphere.hpp
  Objects/MovingSphere.cpp
  Objects/Rectangles.hpp
  Objects/Rectangles.cpp
  Objects/Sphere.hpp
  Objects/Sphere.cpp
  Objects/Transform/FlipFace.hpp
  Objects/Transform/FlipFace.cpp
  Objects/Transform/RotateY.hpp
  Objects/Transform/RotateY.cpp
  Objects/Transform/Translate.hpp
  Objects/Transform/Translate.cpp

  PDFs/CosinePDF.hpp
  PDFs/CosinePDF.cpp
  PDFs/HittablePDF.hpp
  PDFs/HittablePDF.cpp
  PDFs/MixturePDF.hpp
  PDFs/MixturePDF.cpp

  Scenes/All.hpp
  Scenes/Book1.hpp
  Scenes/Book1.cpp
  Scenes/Book2.hpp
  Scenes/Book2.cpp
  Scenes/Book3.hpp
  Scenes/Book3.cpp
  Scenes/Fun.hpp
  Scenes/Fun.cpp

  Textures/CheckerTexture.hpp
  Textures/CheckerTexture.cpp
  Textures/ImageTexture.hpp
  Textures/ImageTexture.cpp
  Textures/NoiseTexture.hpp
  Textures/NoiseTexture.cpp
  Textures/SolidColor.hpp
  Textures/SolidColor.cpp
)
set(third_party_sources
  ../third_party/pcg_extras.hpp
  ../third_party/pcg_random.hpp
  ../third_party/pcg_uint128.hpp
  ../third_party/stb_image.h
  ../third_party/stb_image.c
)

# Get the required libraries
find_package(Threads REQUIRED)

# Copy over assets to the build directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../assets" DESTINATION ${CMAKE_BINARY_DIR})

# Define the library
add_library(PSRayTracing_StaticLibrary STATIC ${sources} ${third_party_sources})
target_include_directories(PSRayTracing_StaticLibrary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})    # Enable includes relative to the root



# Configurable options
option(WITH_SINGLE_PRECISION_REAL "Use `float` for `rreal type`" OFF)
option(WITH_BOOK_CLAMP "Use Book's method for `clamp()` (slow)" OFF)
option(WITH_BOOK_RNG "Use Book's method (meresenne twister) for the random number generator (slow)" OFF)
option(WITH_BOOK_BOX "Use Book's implementation of the `Box` object (Uses `*Rect` objects) (slow)" OFF)
option(WITH_BOOK_AABB_HIT "Use Book's method for AABB-Ray intersection (slow)" OFF)
option(WITH_BOOK_SPHERE_HIT "Use Book's method for Sphere-Ray intersection (slower)" OFF)
option(WITH_BOOK_RECT_HIT "Use Book's method for *Rect-Ray intersection (slower)" OFF)
#option(WITH_BOOK_CONSTANT_MEDIUM_HIT "Use Book's method for ConstantMedium-Ray intersection (slower?)" OFF)
option(WITH_BOOK_DIELECTRIC_SCATTER "Use Book's method for Dielectric scattering (slower)" OFF)
option(WITH_BOOK_SIN_COS "Use Book's method (std::sin, std::cos) for sine and cosine computation (slower, correct)" OFF)
option(WITH_BOOK_ASIN "Use the Book's method (std::asin) for arcsine computation (slower, correct)" OFF)
option(WITH_BOOK_ATAN2 "Use the Book's method (std::atan2) for `atan2()` computation (slower, correct)" OFF)
option(WITH_BOOK_SQRT "Use the Book's method (std::sqrt) for `sqrt()` computation" ON)
option(WITH_BOOK_SCHLICK "Use the Book's method for `schlick()` computation (slower)" OFF)
option(WITH_BOOK_PERLIN "Use the Book's Perlin class" OFF)
option(WITH_BOOK_COMPUTE_PIXEL_COLOR "Use the book's method for computing the color of a pixel" OFF)
option(WITH_BOOK_BVH_NODE "Use the book's method for BVH Nodes (slower?)" ON)
option(WITH_BOOK_MAT_PTR "Use Book's method for HitRecord::mat_ptr, which is a std::shared_ptr (slower)" OFF)
option(WITH_BOOK_PDF_MANAGEMENT "Use the Book's method for managing PDFs, which are pointers (slower)" OFF)
option(WITH_NOEXCEPT "Use the `noexcept` annotation for various functions (faster?)" OFF)
option(WITH_FINAL "Use the `final` specifier on derived classes (faster?)" OFF)



if (WITH_SINGLE_PRECISION_REAL)
  message(STATUS "Using `float` for `rreal` instead of `double`")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_SINGLE_PRECISION_REAL)
else()
  message(STATUS "Using `double` for `rreal`")
endif()

if (WITH_BOOK_CLAMP)
  message(STATUS "Using book's method for clamp (slow)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_CLAMP)
endif()

if (WITH_BOOK_RNG)
  message(STATUS "Using book's method for random number generation (meresenne twister) (slow)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_RNG)
endif()

if (WITH_BOOK_BOX)
  message(STATUS "Using book's implementation of the `Box` object (Uses `*Rect` objects) (slow)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_BOX)
endif()

if (WITH_BOOK_AABB_HIT)
  message(STATUS "Using book's method for AABB-Ray intersection (slow)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_AABB_HIT)
endif()

if (WITH_BOOK_SPHERE_HIT)
  message(STATUS "Using book's method for Sphere-Ray intersection (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_SPHERE_HIT)
endif()

if (WITH_BOOK_RECT_HIT)
  message(STATUS "Using book's method for Rect-Ray intersection (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_RECT_HIT)
endif()

if (WITH_BOOK_SIN_COS)
  message(STATUS "Using book's method (std::sin, std::cos) for sine and cosine computation (slower, correct)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_SIN_COS)
else()
  message(STATUS "Using fast approximation method for sin() and cos() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_ASIN)
  message(STATUS "Using book's method for asin() computation (slower, correct)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_ASIN)
else()
  message(STATUS "Using fast approximation method for asin() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_ATAN2)
  message(STATUS "Using book's method for atan2() computation (slower, correct)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_ATAN2)
else()
  message(STATUS "Using fast approximation method for atan2() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_SQRT)
  message(STATUS "Using book's method for sqrt() computation")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_SQRT)
else()
  message(STATUS "Using fast approximation method for sqrt() computation (acutely incorrect)")
endif()

if (WITH_BOOK_SCHLICK)
  message(STATUS "Using the Book's method for `schlick()` computation (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_SCHLICK)
endif()

if (WITH_BOOK_PERLIN)
  message(STATUS "Using the Book's Perlin class")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_PERLIN)
endif()

if (WITH_BOOK_COMPUTE_PIXEL_COLOR)
  message(STATUS "Using the book's method for computing the color of a pixel")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_COMPUTE_PIXEL_COLOR)
endif()

if (WITH_BOOK_BVH_NODE)
  message(STATUS "Using the book's method for BVH Nodes (slower?)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_BVH_NODE)
endif()

if (WITH_BOOK_MAT_PTR)
  message(STATUS "Using the Book's method for HitRecord::mat_ptr, which is a std::shared_ptr (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_MAT_PTR)
endif()

if (WITH_BOOK_PDF_MANAGEMENT)
  message(STATUS "Using the Book's method for managing PDFs, which are pointers (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_PDF_MANAGEMENT)
else()
  message(STATUS "Using our method for managing PDFs, which is based around std::variant and stack allocations (faster)")
endif()

#if (WITH_BOOK_CONSTANT_MEDIUM_HIT)
#  message(STATUS "Using book's method for ConstantMedium-Ray intersection (slower)")
#  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_CONSTANT_MEDIUM_HIT)
#endif()

if (WITH_BOOK_DIELECTRIC_SCATTER)
  message(STATUS "Using book's method for Dielectric scattering (slower)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_BOOK_DIELECTRIC_SCATTER)
endif()

if (WITH_NOEXCEPT)
  message(STATUS "Using `noexcept` annotations (faster?)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_NOEXCEPT)
else()
  message(STATUS "Turned off use of `noexcept` (slower?)")
endif()

if (WITH_FINAL)
  message(STATUS "Using `final` spicifer (faster?)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_FINAL)
else()
  message(STATUS "Turned off use of `final` (slower?)")
endif()


# Build it!
if (NOT COMPILING_WITH_MSVC)
  # Turn on extra warnings (or supress them) for GCC & clang
  target_compile_options(PSRayTracing_StaticLibrary PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -Wmismatched-tags -Wno-overlength-strings -pedantic)
  set_source_files_properties(${third_party_sources} PROPERTIES COMPILE_FLAGS -w)
endif()

target_link_libraries(PSRayTracing_StaticLibrary
  Threads::Threads
)

# Misc.
set_target_properties(PSRayTracing_StaticLibrary PROPERTIES
  PUBLIC_HEADER "render.hpp"
)
