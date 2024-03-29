cmake_minimum_required(VERSION 3.10)

project(PSRayTracing_CLIRunner)

set(CMAKE_CXX_STANDARD 17)

# So we can use includes relative to the project root
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Get the source
set(sources
  main.cpp

  Misc.hpp
  Misc.cpp
  ProgressBar.hpp
  ProgressBar.cpp
)
set(third_party_sources
  ../third_party/cxxopts.hpp
  ../third_party/stb_image_write.h
  ../third_party/stb_image_write.c
)

# Create the executable
add_executable(PSRayTracing_CLIRunner ${sources} ${third_party_sources})
set_target_properties(PSRayTracing_CLIRunner PROPERTIES
  OUTPUT_NAME "PSRayTracing"
)

# Build it!
target_compile_options(PSRayTracing_CLIRunner PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors)
set_source_files_properties(${third_party_sources} PROPERTIES COMPILE_FLAGS -w)
target_link_libraries(PSRayTracing_CLIRunner
  PSRayTracing_StaticLibrary
)
