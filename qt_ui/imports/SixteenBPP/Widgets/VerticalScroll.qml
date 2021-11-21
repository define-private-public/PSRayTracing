import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import SixteenBPP


/**
 * This is a Control that acts as a container.  It can contain a control that's larger than
 * itself; allowing the user to scoll up and down.  E.g, say the contained control is 1000
 * units tall, but this only has a height of 200, it will show a scrollbar off to the right
 * and the user can flick the container to scroll up and down.  The item you want to contain,
 * set it to be the `content_item` value
 *
 * I had some issues with using the build in ScrollView, so that's why I built this.
 */
Item {
  id: root

  clip: true

  /** This is the object that will be clipped by the container */
  property Item content_item

  readonly property real _padding: 5
  readonly property real visible_area_width: scroll_view.width

  // The actual area where the actual form is
  Flickable {
    id: scroll_view

    clip: true

    anchors.rightMargin: _padding
    anchors.left: parent.left
    anchors.right: scroll_bar.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    contentWidth: width
    contentHeight: content_item.height

    children: [content_item]

    // For some reason, when adding the `content_item` as a child, it's not updating its position.  We have to manually do there here
    //   This feels like a hack...
    visibleArea.onYPositionChanged: content_item.y = (-visibleArea.yPosition * content_item.height);
  }

  // Container for the scrollbar
  // TODO needs mouse support
  Rectangle {
    id: scroll_bar

    anchors.rightMargin: 0
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right

    // Only show the scrollbar where there isn't enough
    visible: (scroll_view.contentHeight > root.height)

    y: 0
    width: 15
    height: parent.height

    color: UITheme.render_display_background_color

    // Scroll for the above flickable
    Rectangle {
      id: scrollbar

      y: (scroll_view.visibleArea.yPosition * root.height)
      width: parent.width
      height: (scroll_view.visibleArea.heightRatio * root.height)

      color: UITheme.black_color
    }
  }
}
