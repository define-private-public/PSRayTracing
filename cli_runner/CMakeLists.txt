cmake_minimum_required(VERSION 3.10)

project(PSRayTracing_CLIRunner)

set(CMAKE_CXX_STANDARD 17)

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
target_include_directories(PSRayTracing_CLIRunner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})    # Enable includes relative to the root
set_target_properties(PSRayTracing_CLIRunner PROPERTIES
  OUTPUT_NAME "PSRayTracing"
)

# Build it!
if (NOT COMPILING_WITH_MSVC)
  # Turn on extra warnings (or supress them) for GCC & clang
  target_compile_options(PSRayTracing_CLIRunner PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors)
  set_source_files_properties(${third_party_sources} PROPERTIES COMPILE_FLAGS -w)
endif()

target_link_libraries(PSRayTracing_CLIRunner
  PSRayTracing_StaticLibrary
)
