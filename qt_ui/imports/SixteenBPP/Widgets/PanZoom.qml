import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SixteenBPP

/**
 * PanZoom is a control that's used to provide a way for the user to zoom in/out on
 * an image, along with panning it up and down.
 *
 * NOTE: for this widget to work correctly you need to specify a `width` and `height`
 *       for it.  You can also use something like `anchors.fill: parent`.  Just as long
 *       as the `width` and `height` fields are defined somehow
 *
 * On touchscreen devices (e.g. Mobile, tablet, & 2-in-1 laptops), they should be able
 * to use "pinch controls" to zoom in/out, and then finger swipes to move up/down/left/right.
 * Double taps (with a finger) should reset the zoom to 100% and center the image
 *
 * On desktop/laptop devices, they should be able to use a mouse wheel to zoom in/out, and
 * click and drag (or flick) with the mouse to move up/down/left/right.  Double clicks should
 * reset the zoom to 100% and center the image.
 *
 * TODO still needs to zoom in/out on a center point.  Right now when zooming in/out, it will
 *      will move the "view window" to the top left.  This isn't good for the user since if they
 *      are pinching to zoom, they might expect it to zoom in/out about the center.  This also
 *      needs to zoom in/out based on where the mouse cursor is (if using a scroll wheel).
 */
Item {
  id: root

  property alias image_source: pan_zoom_target.source   ///< The URL the of the image to be shown
  property alias image_smooth: pan_zoom_target.smooth   ///< Toggles if the image should use flitering/blending to appear smoother when zoomed in

  signal double_tapped    ///< emitted when this has been double clicked/tapped

  readonly property real zoom: (root.pan_zoom_enabled ? __p.zoom : __p.default_zoom)  ///< Get the current zoom factor; use set_zoom() to safely, set the zoom

  // The zoom the fit value is the zoom needed to fit the image perfectly into the bounds of this root
  //   It could be more than 100% (large image), it could be less (small image).  It depends on whatever dimenion is larger
  //   is the largest dimension
  readonly property real zoom_to_fit_ratio: (__p.use_width_ratio ? __p.fit_width_ratio : __p.fit_height_ratio)

  /**
   * When true; this enabled, the panning and zooming features; when disabled, this simply will
   * display the image centered (and scalled down if ti doesn't fit within the width).  By default
   * it is set to `false`
   *
   * Typically when this control is double-tapped, then will be toggled on/off; but that is done
   * where PanZoom is used (e.g. as the child of another Control).
   */
  property bool pan_zoom_enabled: false

  readonly property real _zoom_out_max: (1.0 / 4.0)   ///< The most we can zoom out; semi-private access
  readonly property real _zoom_in_max: 64.0           ///< The most we can zoom in; semi-private access

  // Private internal data structure
  QtObject {
    id: __p

    property real zoom: 1.0

    // The default zoom, that either makes the image fit (it's larger than the widget), or set to 100% (smaller than the wikdget)
    readonly property real default_zoom: ((root.zoom_to_fit_ratio < 1.0) ? root.zoom_to_fit_ratio : 1.0)

    // For figure out zoom ratios;  We want to choose whichever dimension has the smallest ratio, as we will need to size down
    //   the target (image) to fit within these bounds
    readonly property alias target_width: pan_zoom_target.sourceSize.width
    readonly property alias target_height: pan_zoom_target.sourceSize.height
    readonly property real fit_width_ratio: root.width / target_width
    readonly property real fit_height_ratio: root.height / target_height
    readonly property bool use_width_ratio: (fit_width_ratio <= fit_height_ratio)

    // Utility function to clamp value `x` to the range of `[a, b]`
    function clamp(x, a, b)
      { return Math.min(Math.max(x, a), b); }
  }

  // This is so when pan/zoom is turned on, the zoom doesn't "jump" to a different value
  onZoomChanged: {
    if (root.pan_zoom_enabled)
      return;

    // Set the private zoom to be whatever the current zoom is
    __p.zoom = root.zoom;
  }

  /**
   * Sets the zoom to \a z.
   *
   * if \a z is `0.5`, that means zooming out to 50%.  If it is `1.75`, that means to zoom
   * in to 175%.  This will clamp `\z` so that it is in an acceptable range (see the zoom_out/zoom_in
   * max variables).
   *
   * If `pan_zoom_enabled=false`, then this will do nothing.
   */
  function set_zoom(z)
  {
    // Only do something if we're allowed to.
    if (!root.pan_zoom_enabled)
      return;

    __p.zoom = __p.clamp(z, root._zoom_out_max, root._zoom_in_max);
  }

  // Some controls buttons to make life a bit easier
  Rectangle {
    id: controls
    state: 'normal'

    property real _spacing: 8

    visible: root.pan_zoom_enabled

    z: (flicker.z + 1)    // Must be on top always
    anchors.margins: controls._spacing
    anchors.right: parent.right
    anchors.top: parent.top
    width: controls_layout.width + (2 * controls._spacing)
    height: controls_layout.height + (2 * controls._spacing)

    color: UITheme.pan_zoom_controls_background_color
    border.color: UITheme.pan_zoom_controls_border_color
    border.width: 1
    radius: (controls._spacing / 2)

    ColumnLayout {
      id: controls_layout

      anchors.centerIn: parent
      spacing: controls._spacing

      // When clicked, this will adjust the zoom to the "fit" ratio
      Button {
        id: zoom_to_fit_button

        Layout.fillWidth: true

        text: Messages.zoom_to_fit

        onClicked: { root.set_zoom(root.zoom_to_fit_ratio); }
      }

      // When clicked, this should zoom the image to 100% (weather it fits or not)
      Button {
        id: zoom_to_100_button

        Layout.fillWidth: true

        text: Messages.zoom_to_100

        onClicked: { root.set_zoom(1); }
      }
    }

    // We only want to show the controls area (above buttons) when we're moving the pan/zoom target
    //   or have the mouse hoving over it, or zooming in/out
    readonly property bool _show_controls: (
      controls_mouse_area.containsMouse ||
      flicker.dragging ||
      flicker.flicking ||
      pa._pinching ||
      mouse_wheel_handler._zooming
    )

    MouseArea {
      id: controls_mouse_area

      anchors.fill: parent

      hoverEnabled: true

      // Normally MouseArea would "steal" the click events, so the buttons above wouldn't be clickable
      //   But if we do this below, we can have the mouse area only catch the hover events (which is what we want)
      //   and then the click events can be sent to the buttons
      propagateComposedEvents: true
      onClicked:  function(mouse) { mouse.accepted = false; }
      onPressed:  function(mouse) { mouse.accepted = false; }
      onReleased: function(mouse) { mouse.accepted = false; }
    }

    states: [
      // When the mouse isn't over the controls widget, it should be (mostly)faded out
      State {
        name: 'normal'
        when: !controls._show_controls
        PropertyChanges { target: controls;            opacity: 0.3; }
        PropertyChanges { target: zoom_to_fit_button;  opacity: 0.0; }
        PropertyChanges { target: zoom_to_100_button;  opacity: 0.0; }
      },

      // When the mouse is over, the controls widgets, it should be very visible
      State {
        name: 'show_controls'
        when: controls._show_controls
        PropertyChanges { target: controls;            opacity: 0.8; }
        PropertyChanges { target: zoom_to_fit_button;  opacity: 1.0; }
        PropertyChanges { target: zoom_to_100_button;  opacity: 1.0; }
      }
    ]

    transitions: Transition {
      PropertyAnimation {
        properties: 'opacity'
        easing.type: Easing.InOutQuad
        duration: 100
      }
    }
  }

  Flickable {
    id: flicker

    anchors.fill: parent

    interactive: root.pan_zoom_enabled    ///< Disable flicking/panning until it's been turned on
    contentWidth: container.width
    contentHeight: container.height

    // Give us those "pinch to zoom" controls
    PinchArea {
      id: pa

      // The pinch area needs to match that of the "container", so we can't
      //   use anchors here for positining
      x: 0
      y: 0
      width: container.width
      height: container.height

      property real _zoom_at_pinch_start
      property bool _pinching: false        ///< Flag for if we're pinching or not right now

      onPinchStarted: function(pinch) {
        // Zoom only works if `pan_zoom_enabled=true`
        if (!root.pan_zoom_enabled)
          return;

        // Mark we're pinching
        pa._pinching = true;

        // Mark what the zoom value was when the user starts a pinch
        pa._zoom_at_pinch_start = root.zoom;
      }

      onPinchUpdated: function(pinch) {
        // Zoom only works if `pan_zoom_enabled=true`
        if (!root.pan_zoom_enabled)
          return;

        // TODO should there be a "treshold value" (e.g. 5%) that needs to be met before zooming
        //      in/out?  Some users may not like this, other times, it ensures that there are no
        //      accidental zooms

        // Set the new zoom, based on the scale of the pinch event
        var zoom_previous = root.zoom;
        root.set_zoom(pinch.scale * pa._zoom_at_pinch_start);
        var zoom_current = root.zoom;

        pa.better_viewport_zoom_fix(zoom_previous, zoom_current, pinch.center);
      }

      onPinchFinished: function(pinch) {
        pa._pinching = false;
      }

      // Mouse area (used for scroll wheel zoom, double click/tap) needs to live inside of the
      //   pinch area so things work nicely
      MouseArea {
        id: mouse_wheel_handler

        anchors.fill: parent

        property bool _zooming: false;  ///< Flag for if we are doing a zoom right now with the mouse wheel;  Is reset by Timer's timeout

        // Used to reset the `_zooming` property back to false.  Kicked off in the `onWheel()` handler
        Timer {
          id: zooming_reset_timer

          interval: 500   // 1/2th of a second
          onTriggered: { mouse_wheel_handler._zooming = false; }
        }

        onWheel: function(wheel) {
          // Zoom only works if `pan_zoom_enabled=true`
          if (!root.pan_zoom_enabled)
            return;

          // Scroll down (-) => zoom out , scroll up {+) => zoom in
          // If you read the docs for this `angleDelta` thing, you'll find out this works in steps where `120=15 deg`
          // For each full step, we'll increase/degrease by 0.25
          //    Something more user friendly is at higher scalle to have increase/decrease more
          var steps = wheel.angleDelta.y / 120;
          var inc = 0.25 * steps;

          // Set zoom (and record changes)
          var zoom_previous = root.zoom;
          root.set_zoom(root.zoom + inc);
          var zoom_current = root.zoom;

          pa.better_viewport_zoom_fix(zoom_previous, zoom_current, Qt.point(wheel.x, wheel.y));

          // Mark that we're zooming in/out with the wheel (and kick off the reset timer)
          mouse_wheel_handler._zooming = true;
          zooming_reset_timer.restart();
        }

        // This handles both double-clicks (mouse) and double-taps (touchscreen/tablet/phone)
        //   It should reset the zoom to 100%, and position to centered
        onDoubleClicked: {
          root.double_tapped();
        }
      }

      // this will do a better viewport zoom in/out with a focal point
      function better_viewport_zoom_fix(zoom_previous, zoom_current, focal_point)
      {
        // If we're zoomed in (to where the viewport doesn't contain the entire image), we do some special handling to make sure the zoom feels more natural
        var too_big = (flicker.contentWidth > flicker.width) || (flicker.contentHeight > flicker.height);
        if (too_big)
        {
          // Compute the viewport rect we'll use to reset the view (so everyhing is nice and centered)
          var viewport_rect = Qt.rect(flicker.contentX, flicker.contentY, flicker.contentWidth, flicker.contentHeight);
          var zoom_focus = mapToItem(flicker, focal_point);
          var r = g_ui_math_helper.compute_viewport_for_zoom(viewport_rect, zoom_focus, zoom_previous, zoom_current);

          // Use the top-left (origin) point to set the flicker
          flicker.contentX = r.x;
          flicker.contentY = r.y;
        }
      }
    }

    // We want the target of the PanZoom to be centered, so we need to put it inside of a container Item.
    Item {
      id: container

      width: Math.max(flicker.width, pan_zoom_target.zoomedWidth)
      height: Math.max(flicker.height, pan_zoom_target.zoomedHeight)

      // The actualy image (itself) we pan/zoom upon
      Image {
        id: pan_zoom_target

        anchors.centerIn: parent
        width: sourceSize.width
        height: sourceSize.height
        scale: root.zoom

        readonly property real zoomedWidth: (root.zoom * pan_zoom_target.width)
        readonly property real zoomedHeight: (root.zoom * pan_zoom_target.height)

        fillMode: Image.Pad
        mipmap: true          // Smoother scaling
        cache: false          // Required to be able to reload renders
      }
    }
  }
}
