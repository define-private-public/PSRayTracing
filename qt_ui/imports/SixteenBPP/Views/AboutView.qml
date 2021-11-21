import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import SixteenBPP
import SixteenBPP.Widgets


// The "About Page" for the app.  We explain whaqt this is for and
// give some thanks to those who helped us along the way
Item {
  id: root

  signal close_button_clicked();

  QtObject {
    id: __p

    readonly property bool width_is_small: UITheme.is_width_small(root.width)
    readonly property real padding_amount: (width_is_small ? 7.5 : 25)

    // The width of the large whitespace that's to the right/left of the scroller
    readonly property real left_right_scroller_padding: (((root.width - scroller.width) / 2) - (2 * padding_amount))

    // Used to check if there isn't enough room for the close button to in that whitespace off to the right/left
    readonly property bool place_close_button_bellow_scroller: (left_right_scroller_padding < close_button.width);
  }

  Component.onCompleted: {
    // Foward signals
    close_button.clicked.connect(root.close_button_clicked);

//    // Debugging info
//    console.log('w: ' + Screen.width);
//    console.log('h: ' + Screen.height);
//    console.log('dpr: ' + Screen.devicePixelRatio);
  }

  // Title of the form
  Text {
    id: title

    anchors.margins: __p.padding_amount
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top

    text: Messages.about_ps_raytracing
    font.pointSize: UITheme.title_font_size_pt
  }

  VerticalScroll {
    id: scroller

    width: Math.min(UITheme.min_width_threshold, root.width) - (2 * __p.padding_amount);
    anchors.topMargin: __p.padding_amount
    anchors.bottomMargin: __p.padding_amount
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: title.bottom
    anchors.bottom: (__p.place_close_button_bellow_scroller ? close_button.top : parent.bottom)

    content_item: ColumnLayout {
      id: layout

      width:  scroller.visible_area_width

      // Main body of the about page
      Text {
        Layout.fillWidth: true

        text: Messages.about_contents
        wrapMode: Text.WordWrap
        font.pointSize: UITheme.about_font_size_pt

        // Opens the hyperlinks when pressed
        onLinkActivated: Qt.openUrlExternally(link)
      }

      // App Icon
      Image {
        Layout.alignment: Qt.AlignHCenter

        source: 'qrc:/app_icon_compressed'
        sourceSize.width:  ((2 / 3) * parent.width)
        mipmap: true    // Smoother scaling
        fillMode: Image.PreserveAspectFit
      }

      // Info about the app
      Text {
        Layout.fillWidth: true

        text: Messages.info_contents
        wrapMode: Text.WordWrap
        font.pointSize: UITheme.about_font_size_pt
      }
    }
  }


  // Pressing this button should go back to the Render View
  // Note that this adjusts its location (via anchors) if the screen width is too small
  Button {
    id: close_button

    text: Messages.close

    anchors.margins: __p.padding_amount
    anchors.rightMargin: (__p.place_close_button_bellow_scroller ? 0 : __p.padding_amount)
    anchors.right: (__p.place_close_button_bellow_scroller ? scroller.right : parent.right);
    anchors.bottom: parent.bottom
  }
}
