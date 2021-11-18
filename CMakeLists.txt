cmake_minimum_required(VERSION 3.14)

project(PSRayTracing)

# Put all runtime binaries in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# The targets to build
add_subdirectory(render_library)
add_subdirectory(cli_runner)

# Copy over assets to the build directory
file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})
