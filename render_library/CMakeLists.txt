cmake_minimum_required(VERSION 3.10)

project(PSRayTracing_StaticLibrary)

set(CMAKE_CXX_STANDARD 17)

# So we can use includes relative to the project root
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Get the source
set(sources
  render.h
  render.cpp

  AABB.h
  AABB.cpp
  ColorRGBA.h
  ColorRGBA.cpp
  Common.h
  HitRecord.h
  HitRecord.cpp
  ONB.h
  ONB.cpp
  PDFVariant.h
  PDFVariant.cpp
  Perlin.h
  Perlin.cpp
  PerlinReal.h
  PerlinReal.cpp
  RandomGenerator.h
  RandomGenerator.cpp
  Ray.h
  Ray.cpp
  RenderContext.h
  RenderContext.cpp
  RenderMethod.h
  RenderMethod.cpp
  RenderOutput.h
  RenderOutput.cpp
  RenderThread.h
  RenderThread.cpp
  RenderThreadPool.h
  RenderThreadPool.cpp
  SceneDescriptor.h
  ScatterRecord.h
  ScatterRecord.cpp
  Util.h
  Util.cpp
  Vec3.h
  Vec3.cpp

  Cameras/Camera.h
  Cameras/Camera.cpp
  Cameras/MotionBlurCamera.h
  Cameras/MotionBlurCamera.cpp

  Interfaces/ICamera.h
  Interfaces/IDeepCopyable.h
  Interfaces/IHittable.h
  Interfaces/IMaterial.h
  Interfaces/IPDF.h
  Interfaces/ITexture.h

  Materials/Dielectric.h
  Materials/Dielectric.cpp
  Materials/DiffuseLight.h
  Materials/DiffuseLight.cpp
  Materials/Isotropic.h
  Materials/Isotropic.cpp
  Materials/Lambertian.h
  Materials/Lambertian.cpp
  Materials/Metal.h
  Materials/Metal.cpp
  Materials/SurfaceNormal.h
  Materials/SurfaceNormal.cpp

  Objects/Box.h
  Objects/Box.cpp
  Objects/BVHNode.h
  Objects/BVHNode.cpp
  Objects/BVHNode_MorePerformant.h
  Objects/BVHNode_MorePerformant.cpp
  Objects/ConstantMedium.h
  Objects/ConstantMedium.cpp
  Objects/HittableList.h
  Objects/HittableList.cpp
  Objects/MovingSphere.h
  Objects/MovingSphere.cpp
  Objects/Rectangles.h
  Objects/Rectangles.cpp
  Objects/Sphere.h
  Objects/Sphere.cpp
  Objects/Transform/FlipFace.h
  Objects/Transform/FlipFace.cpp
  Objects/Transform/RotateY.h
  Objects/Transform/RotateY.cpp
  Objects/Transform/Translate.h
  Objects/Transform/Translate.cpp

  PDFs/CosinePDF.h
  PDFs/CosinePDF.cpp
  PDFs/HittablePDF.h
  PDFs/HittablePDF.cpp
  PDFs/MixturePDF.h
  PDFs/MixturePDF.cpp

  Scenes/All.h
  Scenes/Book1.h
  Scenes/Book1.cpp
  Scenes/Book2.h
  Scenes/Book2.cpp
  Scenes/Book3.h
  Scenes/Book3.cpp
  Scenes/Fun.h
  Scenes/Fun.cpp

  Textures/CheckerTexture.h
  Textures/CheckerTexture.cpp
  Textures/ImageTexture.h
  Textures/ImageTexture.cpp
  Textures/NoiseTexture.h
  Textures/NoiseTexture.cpp
  Textures/SolidColor.h
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
option(WITH_BOOK_BVH_NODE "Use the book's method for BVH Nodes (slower)" OFF)
option(WITH_BOOK_MAT_PTR "Use Book's method for HitRecord::mat_ptr, which is a std::shared_ptr (slower)" OFF)
option(WITH_BOOK_PDF_MANAGEMENT "Use the Book's method for managing PDFs, which are pointers (slower)" OFF)
option(WITH_NOEXCEPT "Use the `noexcept` annotation for various functions (faster)" ON)



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
  message(STATUS "Using the book's method for BVH Nodes (slower)")
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
  message(STATUS "Using `noexcept` annotations (faster)")
  target_compile_definitions(PSRayTracing_StaticLibrary PUBLIC USE_NOEXCEPT)
else()
  message(STATUS "Turned off use of `noexcept` (slower)")
endif()


# Build it!
target_compile_options(PSRayTracing_StaticLibrary PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -Wno-overlength-strings -pedantic)
set_source_files_properties(${third_party_sources} PROPERTIES COMPILE_FLAGS -w)
target_link_libraries(PSRayTracing_StaticLibrary Threads::Threads)

# Misc.
set_target_properties(PSRayTracing_StaticLibrary PROPERTIES
  PUBLIC_HEADER "render.h"
)
