package net.sixteenbpp.psraytracing;

import android.content.Context;
import android.graphics.Point;
import android.util.DisplayMetrics;


public class AndroidUtils
{
    /**
     * This is an Android native method we need to call to get the physical screen size
     * (in pixels) of our device.
     *
     * \a context must be the active application context.
     */
    public static Point get_device_resolution_in_pixels(Context context)
    {
        // NOTE: This API is deprecated in android API 30 and above.
        //       But I don't see any other way of getting the "real size" (easily) in the
        //       new recommended API
        Point dim = new Point();
        context.getDisplay().getRealSize(dim);
        return dim;
    }
}
