#!/bin/bash

# PSRayTracing Android Release Build Script
# This script configures and builds a multi-ABI Android App Bundle (.aab) for Google Play.

set -e

# --- Configuration ---
# Update these paths to match your system environment
export ANDROID_SDK_ROOT=${ANDROID_SDK_ROOT:-"/Users/ben/Library/Android/sdk"}
export ANDROID_NDK_ROOT=${ANDROID_NDK_ROOT:-"/Users/ben/Library/Android/sdk/ndk/27.2.12479018"}
# Path to your Qt installation (using your specific local path)
export QT_PATH=${QT_PATH:-"/Users/ben/dev_tools/6.10.2/android_arm64_v8a"} 

PROJECT_ROOT=$(pwd)
BUILD_DIR="$PROJECT_ROOT/build-android-release"
KEYSTORE_PATH="$PROJECT_ROOT/psraytracing_android_release.keystore"
KEYSTORE_ALIAS="psraytracing_android_key" # Updated to the correct alias found in the keystore

# 0. Get Passwords (to avoid multiple prompts and ensure environment is set)
echo -n "Enter Keystore Password: "
read -s KEYSTORE_PASS
echo ""

# Export signing variables so androiddeployqt can find them automatically
export QT_ANDROID_KEYSTORE_PATH="$KEYSTORE_PATH"
export QT_ANDROID_KEYSTORE_ALIAS="$KEYSTORE_ALIAS"
export QT_ANDROID_KEYSTORE_STORE_PASS="$KEYSTORE_PASS"
export QT_ANDROID_KEYSTORE_KEY_PASS="$KEYSTORE_PASS"

# ABIs to include in the bundle (Google Play standard)
# Including 32-bit (v7 and x86) for maximum compatibility
ANDROID_ABIS="armeabi-v7a;arm64-v8a;x86;x86_64"

echo "--------------------------------------------------------"
echo " PSRayTracing: Starting Android Release Build"
echo " Target ABIs: $ANDROID_ABIS"
echo "--------------------------------------------------------"

# 1. Validation
if [ ! -d "$QT_PATH" ]; then
    echo "ERROR: QT_PATH not found at $QT_PATH"
    echo "Please ensure the path to your Qt Android installation is correct."
    exit 1
fi

if [ ! -f "$KEYSTORE_PATH" ]; then
    echo "ERROR: Keystore not found at $KEYSTORE_PATH"
    exit 1
fi

if [ ! -d "$ANDROID_NDK_ROOT" ]; then
    echo "ERROR: NDK not found at $ANDROID_NDK_ROOT"
    exit 1
fi

# 2. Setup Build Directory
echo "--> Cleaning and creating build directory: $BUILD_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 3. Configure with CMake
# We use -DQT_ANDROID_ABIS to tell Qt to build for multiple architectures at once.
# We MUST specify the CMAKE_TOOLCHAIN_FILE for cross-compilation to work.
# We switch to Ninja as the generator (-GNinja) as it's the standard and more reliable for multi-ABI builds.
echo "--> Configuring CMake with Ninja..."
cmake -GNinja "$PROJECT_ROOT" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$QT_PATH/lib/cmake/Qt6/qt.toolchain.cmake" \
    -DQT_HOST_PATH="/Users/ben/dev_tools/6.10.2/macos" \
    -DANDROID_SDK_ROOT="$ANDROID_SDK_ROOT" \
    -DANDROID_NDK_ROOT="$ANDROID_NDK_ROOT" \
    -DQT_ANDROID_ABIS="$ANDROID_ABIS" \
    -DQT_ANDROID_SIGN_APK=ON \
    -DQT_ANDROID_SIGN_AAB=ON

# 4. Build the App Bundle
# The 'aab' target is the standard target. Release mode is handled by CMAKE_BUILD_TYPE
echo "--> Building Android App Bundle (AAB)..."
cmake --build . --target aab -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)

# 5. Manual Signing and Verification
# Sometimes Qt/Gradle doesn't correctly sign the .aab itself. We'll do it manually to be 100% sure.
echo "--> Finding and manually signing the .aab file..."
AAB_FILE=$(find . -name "*.aab" | head -n 1)

if [ -z "$AAB_FILE" ]; then
    echo "ERROR: Could not find generated .aab file!"
    exit 1
fi

echo "Signing: $AAB_FILE"
jarsigner -keystore "$KEYSTORE_PATH" -storepass "$KEYSTORE_PASS" "$AAB_FILE" "$KEYSTORE_ALIAS"

echo "--> Verifying the signature..."
jarsigner -verify "$AAB_FILE" | tail -n 2

# 6. Size Analysis
echo "--> Analyzing bundle size..."
echo "Total Size: $(du -h "$AAB_FILE" | cut -f1)"
echo "Top 10 largest files in bundle (to check for debug symbols):"
unzip -l "$AAB_FILE" | sort -n -r -k1 | head -n 10

echo "--------------------------------------------------------"
echo " Build and Signing Complete!"
echo "--------------------------------------------------------"
echo " Your signed release bundle is located at:"
echo "$BUILD_DIR/$AAB_FILE"
echo ""
echo " Next Steps:"
echo " 1. Upload the .aab file to the Google Play Console."
echo "--------------------------------------------------------"
