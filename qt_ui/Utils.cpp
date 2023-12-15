#include "Utils.hpp"
#include <QGuiApplication>
#include <QScreen>

#if defined(Q_OS_ANDROID)
    #include <QJniObject>
#elif defined(Q_OS_IOS)
    #include "apple/iOSUtils.h"
#endif


#if defined(Q_OS_ANDROID)
    QSize android_get_device_resolution_in_pixels()
    {
        // Call our Java/Android code
        const QJniObject dim = QJniObject::callStaticObjectMethod(
            "net/sixteenbpp/psraytracing/AndroidUtils",
            "get_device_resolution_in_pixels",
            "(Landroid/content/Context;)Landroid/graphics/Point;",
            QNativeInterface::QAndroidApplication::context()
        );
    
        // Get the result
        return QSize(
            dim.getField<jint>("x"),
            dim.getField<jint>("y")
        );
    }
#endif


QSize utils::compute_true_screen_resolution()
{
    // This is a Qt (6.2.1) quirk (for now...):
    //
    // Since Qt, Qml's Screen, QScreen don't provide a method to get the actual physical
    // amount of pixels that are the true screen resolution.  So instead for certain
    // platforms we need to do different things.
    //
    // Part of this issue stems from high-dpi displays and Qt trying to make apps
    // scale up well.  I have an Android LGQ7+ with a native resolution of `2160x1080`,
    // But Qml' Screen (and QScreen) is reporting it to be something like `823x411`.
    //
    // But also other APIs now have a "safe area" and those APIs that you used to querey
    // the screen resolution now returns the "safe area" resolution.
    //
    // Desktop OSs (Windows, Mac, Linux):
    //   It seems like we can use the `QScreen` API as is.  I haven't tested this on
    //   macOS Retina displays or the new MacBook pro that has the "notch".  I haven't
    //   seen any issues with non-HighDPI displays.  This also worked with a Windows 2-in-1
    //   that can rotate.
    //
    // Android:
    //   The QScreen API is returning a scalled value, as well as one that falls in the
    //   "safe area" of the screen.  It doesn't include the back/home/menu buttons that
    //   appear at the bottom, or the time/date/battery status bar at the top.  For this
    //   We need to do an Android JNI call.  The code that I wrote is unfortunately depcreated
    //   since Android API 30, but I'm using it as I haven't seen any substition code
    //   that could be used instead.
    //
    // iOS:
    //   The major quirk here is that you need to set a "Launch Screen" for your app or
    //   it will not be the correct resolution, nor will any of the APIs to query the screen
    //   resolution work property.  This includes both Qt's QScreen and the Apple UIKit
    //   ones (!!).  I have an iPad Pro 2021 (12.9 in).  When I didn't have a Launch Screen
    //   set, both APIs were giving me a value that was 2/3 of the true native resolution;
    //   Once I had one set, then both using QScreen or UIKit worked as expected.
    //
    //   This also wasn't documented anywhere, I had to go searching through Google and
    //   forums posts to figure this out.  While the Apple Human Interface guidlines do state
    //   that an app needs to have a launch screen set, it isn't obvious to anyone that
    //   this also sets what resolution your app will run in.  The UIKit documentation for
    //   polling for the native resolution doesn't say anything, nor does the Qt stuff.
    //   </rant>
    //
    //   While I stated above that QScreen will work with iOS (from initial testing), I'm
    //   keeping in the iOS native calls to serve as an example of how to add them to a
    //   cross platform Qt app.


    #if defined(Q_OS_ANDROID)
        return android_get_device_resolution_in_pixels();
    #elif defined(Q_OS_IOS)
        return ios_utils::get_device_resolution_in_pixels();
    #else
        // Fallback (or Desktop OSes)
        // TODO test on macOS Retina, and those with a notch
        const QScreen *screen = QGuiApplication::primaryScreen();
        const QSizeF dim(screen->size());
        return (screen->devicePixelRatio() * dim).toSize();
    #endif
}
