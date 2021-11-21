#pragma once

#include <QSize>

namespace ios_utils
{
    /** Calls some native iOS stuff to get the actual screen resolution of the device */
    QSize get_device_resolution_in_pixels();
}
