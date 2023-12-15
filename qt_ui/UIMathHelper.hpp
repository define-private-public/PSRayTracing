#pragma once

#include <QObject>
#include <QRectF>


/**
 * UIMathHelper is an object that houses methods for doing math for the UI.
 *
 * E.g. doing the computations for zoom in/out and positioning.  QRect has a
 * nice API for math operations, but Qml's `rect` type doesn't.   So this
 * class exists so we can leverage that more advanced API in Qml.
 *
 * It would be best to treat this object as a singleton (at least in Qml land).
 */
class UIMathHelper : public QObject
{
    Q_OBJECT

public:
    explicit UIMathHelper(QObject *parent = nullptr);
    ~UIMathHelper();

    /// Computes the viewport that should be used when zooming in or out, so things look nice and centered.
    Q_INVOKABLE static QRectF compute_viewport_for_zoom(
        const QRectF &current_viewport,                         // Value could be zoomed
        const QPointF &zoom_focus,                              // Value is in "widget space"
        const qreal zoom_before,                                // value is a percentage eg. `0.5 = 50%`
        const qreal zoom_after                                  // Value is a percentage eg. `0.5 = 50%`
    );
};
