#include "UIMathHelper.hpp"

UIMathHelper::UIMathHelper(QObject *parent) :
    QObject(parent)
{ }

UIMathHelper::~UIMathHelper() = default;


QRectF UIMathHelper::compute_viewport_for_zoom(
    const QRectF &viewport_rect,
    const QPointF &zoom_focus,
    const qreal zoom_previous,
    const qreal zoom_current
) {
    // If the zooms are the same, no need to change the offset
    if (qFuzzyCompare(zoom_previous, zoom_current) || (zoom_previous <= 0)) {
        return viewport_rect;
    }

    // Zoom from top-left logic:
    // NewContentX = (OldContentX + focusX) * (zoom_current / zoom_previous) - focusX
    //   Where OldContentX is the scroll/flicker offset, and focusX is the screen position of the focal point
    const qreal ratio = zoom_current / zoom_previous;
    const qreal new_content_x = (viewport_rect.x() + zoom_focus.x()) * ratio - zoom_focus.x();
    const qreal new_content_y = (viewport_rect.y() + zoom_focus.y()) * ratio - zoom_focus.y();

    // Return the new offset (as the top-left) and the original size
    return QRectF(new_content_x, new_content_y, viewport_rect.width(), viewport_rect.height());
}
