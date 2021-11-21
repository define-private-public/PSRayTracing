cmake_minimum_required(VERSION 3.14)

project(PSRayTracing)

# Check if we're building for iOS (and put it into a more friendly variable)
set(IOS FALSE)
if (${CMAKE_SYSTEM_NAME} STREQUAL iOS)
  set(IOS TRUE)
endif()

# If we're building for Android or iOS, turn on the UI
set(IS_MOBILE FALSE)
if (ANDROID OR IOS)
  set(IS_MOBILE TRUE)
endif()



# Configurable options
option(BUILD_QT_UI "Build the Qt UI as well (requires Qt 6)" ${IS_MOBILE})

# Put all runtime binaries in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Always build the render library
add_subdirectory(render_library)

# If weren't not building for a mobile device, then build the CLI
if (NOT IS_MOBILE)
  add_subdirectory(cli_runner)
endif()

# Also build the Qt UI?
if (BUILD_QT_UI)
  add_subdirectory(qt_ui)
endif()

# Copy over assets to the build directory
file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})
