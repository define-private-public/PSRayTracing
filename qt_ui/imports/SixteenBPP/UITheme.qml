pragma Singleton

import QtQuick
import QtQuick.Window


// These are themeing constants that are used throughout the app.
// These are things such as colour choices, images, etc.   It's possible
// in the future we want to be able to theme the app, so centralizing
// where all of the colors go makes a lot of sense
QtObject {
  id: root

  readonly property real _scaling_factor: Math.min(1.8, Screen.devicePixelRatio)

  readonly property string settings_icon:   'qrc:/settings_icon'
  readonly property string start_icon:      'qrc:/start_icon'
  readonly property string stop_icon:       'qrc:/stop_icon'

  readonly property color white_color:                         '#FFFFFF'
  readonly property color black_color:                         '#000000'
  readonly property color render_display_background_color:     '#CCCCCC'
  readonly property color scrollbar_color:                     '#565656'
  readonly property color pan_zoom_controls_background_color:  '#222222'
  readonly property color pan_zoom_controls_border_color:      '#CCCCCC'

  readonly property real title_font_size_pt: _scaling_factor * 18
  readonly property real about_font_size_pt: _scaling_factor * _about_font_size_base

  // TODO document better
  // This is for smartphone's and small screen, If they have a screen width less than this
  //   we adjust the layout for that kind of display
  readonly property int min_width_threshold: 700

  // Depending upon the language, we need to make tha bout text a bit bigger (maybe)
  readonly property real _about_font_size_base: {
    var is_japanese = Qt.locale().name.startsWith('ja');    // Most likely `ja_JP`, but I once got `ja_US`...
    return (is_japanese ? 15 : 12);
  }

//  readonly property bool width_is_small: Screen.width <= min_width_threshold

  /** Checks to see if a given width is considered "small" for the theme */
  function is_width_small(width)
    { return (width < root.min_width_threshold); }
}
