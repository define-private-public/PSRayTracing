cmake_minimum_required(VERSION 3.10)

project(PSRayTracing)

set(CMAKE_CXX_STANDARD 17)

# So we can use includes relative to the project root
include_directories(${CMAKE_SOURCE_DIR})
#include_directories("${CMAKE_SOURCE_DIR}/third_party/")

# Get the source
file(GLOB_RECURSE sources
    "*.cpp"
    "Interfaces/**"
    "Materials/**"
    "Objects/**"
    "Objects/**"
    "Cameras/**"
    "Scenes/**"
    "Util/**"
    "Textures/**"
)
file(GLOB third_party_sources "third_party/**")

# Get the required libraries
find_package(Threads REQUIRED)
find_package(Boost 1.65 REQUIRED COMPONENTS program_options)
#find_package(OpenMP REQUIRED)

# Copy over assets to the build directory
#file(MAKE_DIRECTORY "assets")
file(COPY "${CMAKE_SOURCE_DIR}/../assets" DESTINATION ${CMAKE_BINARY_DIR})


# Define the executable
add_executable(PSRayTracing ${sources} ${third_party_sources})

# Configurable options
option(WITH_SINGLE_PRECISION_REAL "Use `float` for `rreal type`" OFF)
option(WITH_BOOK_CLAMP "Use Book's method for `clamp()` (slow)" OFF)
option(WITH_BOOK_RNG "Use Book's method (meresenne twister) for the random number generator (slow)" OFF)
option(WITH_BOOK_BOX_HEIARCHY "Use Book's method (`HittableList`) for storing a `Box`'s child rectangles (instead of a slightly more optimized `BVHNode`)" OFF)
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
option(WITH_NOEXCEPT "Use the `noexcept` annotation for various functions (faster)" ON)

if (WITH_SINGLE_PRECISION_REAL)
  message(STATUS "Using `float` for `rreal` instead of `double`")
  target_compile_definitions(PSRayTracing PUBLIC USE_SINGLE_PRECISION_REAL)
else()
  message(STATUS "Using `double` for `rreal`")
endif()

if (WITH_BOOK_CLAMP)
  message(STATUS "Using book's method for clamp (slow)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_CLAMP)
endif()

if (WITH_BOOK_RNG)
  message(STATUS "Using book's method for random number generation (meresenne twister) (slow)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_RNG)
endif()

if (WITH_BOOK_BOX_HEIARCHY)
  message(STATUS "Using book's method for storing children rectangles in a `Box` (`HittableList` instead of `BVHNode`)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_BOX_HEIARCHY)
endif()

if (WITH_BOOK_AABB_HIT)
  message(STATUS "Using book's method for AABB-Ray intersection (slow)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_AABB_HIT)
endif()

if (WITH_BOOK_SPHERE_HIT)
  message(STATUS "Using book's method for Sphere-Ray intersection (slower)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_SPHERE_HIT)
endif()

if (WITH_BOOK_RECT_HIT)
  message(STATUS "Using book's method for Rect-Ray intersection (slower)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_RECT_HIT)
endif()

if (WITH_BOOK_SIN_COS)
  message(STATUS "Using book's method (std::sin, std::cos) for sine and cosine computation (slower, correct)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_SIN_COS)
else()
  message(STATUS "Using fast approximation method for sin() and cos() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_ASIN)
  message(STATUS "Using book's method for asin() computation (slower, correct)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_ASIN)
else()
  message(STATUS "Using fast approximation method for asin() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_ATAN2)
  message(STATUS "Using book's method for atan2() computation (slower, correct)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_ATAN2)
else()
  message(STATUS "Using fast approximation method for atan2() computation (faster, acutely incorrect)")
endif()

if (WITH_BOOK_SQRT)
  message(STATUS "Using book's method for sqrt() computation")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_SQRT)
else()
  message(STATUS "Using fast approximation method for sqrt() computation (acutely incorrect)")
endif()

if (WITH_BOOK_SCHLICK)
  message(STATUS "Using the Book's method for `schlick()` computation (slower)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_SCHLICK)
endif()

if (WITH_BOOK_PERLIN)
  message(STATUS "Using the Book's Perlin class")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_PERLIN)
endif()

if (WITH_BOOK_COMPUTE_PIXEL_COLOR)
  message(STATUS "Using the book's method for computing the color of a pixel")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_COMPUTE_PIXEL_COLOR)
endif()


#if (WITH_BOOK_CONSTANT_MEDIUM_HIT)
#  message(STATUS "Using book's method for ConstantMedium-Ray intersection (slower)")
#  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_CONSTANT_MEDIUM_HIT)
#endif()

if (WITH_BOOK_DIELECTRIC_SCATTER)
  message(STATUS "Using book's method for Dielectric scattering (slower)")
  target_compile_definitions(PSRayTracing PUBLIC USE_BOOK_DIELECTRIC_SCATTER)
endif()

if (WITH_NOEXCEPT)
  message(STATUS "Using `noexcept` annotations (faster)")
  target_compile_definitions(PSRayTracing PUBLIC USE_NOEXCEPT)
else()
  message(STATUS "Turned off use of `noexcept` (slower)")
endif()


# Build it!
target_compile_options(PSRayTracing PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors)
set_source_files_properties(${third_party_sources} PROPERTIES COMPILE_FLAGS -w)
#target_link_libraries(RayTracing OpenMP::OpenMP_CXX Boost::program_options)
target_link_libraries(PSRayTracing Threads::Threads Boost::program_options)
