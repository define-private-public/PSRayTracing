# Some bash scripts/aliases that make development a bit easier

export PSRT_PROJECT_ROOT=`pwd`

# NOTE: these may be different for different build directories on different sytems...
export ANDROID_BUILD_DIR=${PSRT_PROJECT_ROOT}/build-android-v7
export LINUX_BUILD_DIR=${PSRT_PROJECT_ROOT}/build-linux-qt


alias create_build_directores='mkdir ${LINUX_BUILD_DIR} && mkdir ${ANDROID_BUILD_DIR}'


# TODO: this isn't propertly rebuilding when the QML  files are changed.  The deploy works, but not the build part...
# Command to push the .apk to android via CLI
alias push_to_android='adb install ${ANDROID_BUILD_DIR}/qt_ui/android-build/build/outputs/apk/debug/android-build-debug.apk'
alias build_and_push_to_android='cmake --build ${ANDROID_BUILD_DIR} && push_to_android'


alias build_and_run_linux_ui='cmake --build ${LINUX_BUILD_DIR} && ${LINUX_BUILD_DIR}/PSRayTracing_QtUI'

