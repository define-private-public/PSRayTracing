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
    // If we are zoomed out (or at 100%), then we just set the top-left to zero so it centers the image
    const bool is_one = qFuzzyCompare(zoom_current, 1.0);
    if ((zoom_current < 1.0) || (is_one))
        return QRectF(QPointF(0, 0), viewport_rect.size());

    // Figure out the viewport, in raw pixel sizes
    const QSizeF viewport_pixel_size = viewport_rect.size() / zoom_current;

    // Compute dimensions of previous viewport (in "non-zoomed-in space")
    const QPointF top_left_previous = viewport_rect.topLeft() / zoom_previous;
    const QSizeF size_previous = viewport_pixel_size / zoom_previous;
    const QRectF viewport_previous(top_left_previous, size_previous);

    // Compute dimensions for current viewport (in "non-zoomed-in space")
    const QSizeF size_current = viewport_pixel_size / zoom_current;
    const QPointF viewport_top_left = viewport_rect.topLeft() / zoom_current;

    // Center the two viewports
    QRectF viewport_current(viewport_top_left, size_current);
//    viewport_current.moveCenter(viewport_previous.center());

    // Compute the focal point (in a range of [0.0, 1.0] in the pixel space)
    const QPointF zoom_focus_percent(
        zoom_focus.x() / viewport_pixel_size.width(),
        zoom_focus.y() / viewport_pixel_size.height()
    );

    // The focal points of the two viewport rects need to overlap.  Then we compute the top-left point
    //   we need for the current viewport
    auto f = [](const qreal p, const qreal s, const qreal d1, const qreal d2)
        { return p + (s * (d1 - d2)); };
    const QPointF tl(
        f(viewport_previous.x(), zoom_focus_percent.x(), viewport_previous.width(), viewport_current.width()),
        f(viewport_previous.y(), zoom_focus_percent.y(), viewport_previous.height(), viewport_current.height())
    );
    viewport_current.moveTopLeft(tl);

    // Make sure it's still in bounds
    const qreal w = viewport_pixel_size.width();
    const qreal h = viewport_pixel_size.height();
    if (viewport_current.left() < 0)
        viewport_current.moveLeft(0);
    if (viewport_current.top() < 0)
        viewport_current.moveTop(0);
    if (viewport_current.right() > w)
        viewport_current.moveRight(w);
    if (viewport_current.bottom() > h)
        viewport_current.moveBottom(h);

    // Return a rectangle back, with the zoom applied
    return QRectF(
      zoom_current * viewport_current.topLeft(),
      viewport_rect.size()
    );
}
