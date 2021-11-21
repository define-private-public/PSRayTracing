#include "iOSUtils.h"
#import <UIKit/UIScreen.h>
#import <UIKit/UIDevice.h>


// I want to note that We could have used the QScreen API here instead, but I'd like to keep an example
// in this codebase of how to do iOS native calls.


QSize ios_utils::get_device_resolution_in_pixels()
{
    // `nativeBounds` on the `mainScreen` is the property we need to access to get the true pixel size,
    // but for this to work correctly you will need to add a "Launch Screen" to the app.  Else, this
    // will report an incorrect scaled (down) value.
    const CGRect dim = [UIScreen mainScreen].nativeBounds;

    QSize native_size(
        static_cast<int>(dim.size.width),
        static_cast<int>(dim.size.height)
    );

    // `nativeBounds` returns the value as if the device in Portrait mode.  We Need to check the orientation
    // and manually "rotate" the size above if we are in landcape

    if (UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation))
        native_size.transpose();

    return native_size;
}
