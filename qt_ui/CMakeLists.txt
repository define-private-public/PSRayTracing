cmake_minimum_required(VERSION 3.14)

project(PSRayTracing_QtUI VERSION 1.3)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 COMPONENTS Core Quick Svg REQUIRED)
find_package(Qt6 COMPONENTS Core Quick Svg Linguist REQUIRED)

# We might need some native code for certain functionality
if (ANDROID)
  set(PLATFORM_SPECIFIC_SOURCES android/src/net/sixteenbpp/psraytracing/AndroidUtils.java)
elseif (IOS)
  set(PLATFORM_SPECIFIC_SOURCES
      apple/iOSUtils.h
      apple/iOSUtils.mm
      apple/Launch.storyboard       # Required so we can use native resolution (no scaling!)
  )
endif()


set(SOURCES
  main.cpp
  PSRayTracingRenderer.hpp
  PSRayTracingRenderer.cpp
  Settings.hpp
  Settings.cpp
  UIMathHelper.hpp
  UIMathHelper.cpp
  Utils.hpp
  Utils.cpp

  ${PLATFORM_SPECIFIC_SOURCES}

  qml.qrc
  qtquickcontrols2.conf
)

qt_add_executable(PSRayTracing_QtUI
  MANUAL_FINALIZATION
  ${SOURCES}
)
set_property(TARGET PSRayTracing_QtUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

# Add files for translations
qt_add_translations(PSRayTracing_QtUI
  TS_FILES
  i18n/qt_ui_en_US.ts
  i18n/qt_ui_de_DE.ts
  i18n/qt_ui_ja_JP.ts
)
add_dependencies(PSRayTracing_QtUI release_translations)
# Note that `update_translations` is not inclued above as a dependency;
#   It seems to be destroying the `.ts` files in the source...  So we'll
#   run `lupdate` manually ourselves.  No biggie


target_compile_definitions(PSRayTracing_QtUI
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(PSRayTracing_QtUI PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Svg
  PSRayTracing_StaticLibrary
)

qt_import_qml_plugins(PSRayTracing_QtUI)



if (APPLE)
  # Required (for on Apple devices) to get an app icon
  set(APPLE_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/apple/Assets.xcassets)
  target_sources(PSRayTracing_QtUI PRIVATE ${APPLE_ASSETS})
  set_source_files_properties(${APPLE_ASSETS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  set(APPLE_BUNDLE_ID "net.16bpp.psraytracing")
  set(APPLE_EXE_NAME "PSRayTracing")
  set(APPLE_SHORT_VERSION_STRING "${PROJECT_VERSION}.0")    # Apple requires a "major.minor.patch" string (e.g. "1.2.3"), but I'm only doing "major.minor" (e.g. "1.2")

  set_target_properties(PSRayTracing_QtUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ${APPLE_BUNDLE_ID}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_EXECUTABLE_NAME ${APPLE_EXE_NAME}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${APPLE_SHORT_VERSION_STRING}
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/apple/Info.plist.in

    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${APPLE_BUNDLE_ID}      # Fixes an XCode warning
    XCODE_ATTRIBUTE_EXECUTABLE_NAME ${APPLE_EXE_NAME}                 # Makes it so that our app name shows up as "PSRayTracing", without the `_QtUI` at the nd
    XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"      # Required to make sure our AppIcon works
  )
elseif (ANDROID)

  # Because of Qt 6.2.2, we must publish single ABI builds.  Qt, in their official docs, recommends doing this:
  #   https://doc.qt.io/qt-6.2/android-publishing-to-googleplay.html (see section about "Publishing Single-ABI bundles")
  #
  # To fill out the version code.  I think it work well, but sadly, I haven't found any nice things in the Qt docs to build
  # this version code automatically, so we need to manually it ourselves...
  #
  # FYI, CMake, or something else is filtering out that leading `0` for ARM stuff.

  # Figure ou the platform & architecture code we need
  set(ANDROID_PLATFORM_ARCHITECTURE_CODE "xxx")
  if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ANDROID_PLATFORM_ARCHITECTURE_CODE "032")
  elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ANDROID_PLATFORM_ARCHITECTURE_CODE "064")
  elseif (${ANDROID_ABI} STREQUAL "x86")
    set(ANDROID_PLATFORM_ARCHITECTURE_CODE "132")
  elseif (${ANDROID_ABI} STREQUAL "x86_64")
    set(ANDROID_PLATFORM_ARCHITECTURE_CODE "164")
  endif()

  # Slap the above together with a version code (major and minor only)
  set(ANDROID_VERSION_CODE "${ANDROID_PLATFORM_ARCHITECTURE_CODE}${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR}")
  set_property(TARGET PSRayTracing_QtUI APPEND PROPERTY QT_ANDROID_VERSION_CODE ${ANDROID_VERSION_CODE})
  set_property(TARGET PSRayTracing_QtUI APPEND PROPERTY QT_ANDROID_VERSION_NAME "${PROJECT_VERSION}")

  set_target_properties(PSRayTracing_QtUI PROPERTIES
    QT_ANDROID_MIN_SDK_VERSION "33"
  )
endif()

# We need to manually finalize since we wanted the Android stuff
qt_finalize_executable(PSRayTracing_QtUI)
