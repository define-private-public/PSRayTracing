pragma Singleton

import QtQuick
import QtQuick.Window


// These are themeing constants that are used throughout the app.
// These are things such as colour choices, images, etc.   It's possible
// in the future we want to be able to theme the app, so centralizing
// where all of the colors go makes a lot of sense
QtObject {
  id: root

  readonly property string settings_icon:   'qrc:/settings_icon'
  readonly property string start_icon:      'qrc:/start_icon'
  readonly property string stop_icon:       'qrc:/stop_icon'

  // Detection of system dark mode (available in Qt 6.5+)
  // We'll fall back to light mode if it's not detected
  readonly property bool is_dark_mode: (Qt.styleHints.colorScheme === Qt.Dark)

  // Semantic color scheme that adapts to light/dark mode
  readonly property color main_background_color: is_dark_mode ? '#121212' : '#FFFFFF'
  readonly property color secondary_background_color: is_dark_mode ? '#1E1E1E' : '#F5F5F5'
  readonly property color main_text_color: is_dark_mode ? '#E0E0E0' : '#212121'
  readonly property color secondary_text_color: is_dark_mode ? '#AAAAAA' : '#666666'
  readonly property color accent_color: is_dark_mode ? '#4CAF50' : '#2E7D32'
  readonly property color divider_color: is_dark_mode ? '#333333' : '#E0E0E0'

  // Material-specific theme constants (if using Material style)
  readonly property int material_theme: is_dark_mode ? 1 /*Material.Dark*/ : 0 /*Material.Light*/
  readonly property color material_accent: '#4CAF50'
  readonly property color material_primary: is_dark_mode ? '#222222' : '#FFFFFF'

  // Original colors kept for compatibility or specific uses
  readonly property color white_color:                         '#FFFFFF'
  readonly property color black_color:                         '#000000'
  readonly property color render_display_background_color:     is_dark_mode ? '#222222' : '#CCCCCC'
  readonly property color scrollbar_color:                     is_dark_mode ? '#888888' : '#565656'
  readonly property color pan_zoom_controls_background_color:  is_dark_mode ? '#000000' : '#222222'
  readonly property color pan_zoom_controls_border_color:      is_dark_mode ? '#444444' : '#CCCCCC'

  // Font sizes increased slightly for better visibility on high-DPI mobile devices
  // Especially on Android where users reported it feeling small.
  readonly property real title_font_size_pt: (Qt.platform.os === 'android' ? 22 : 18)
  readonly property real about_font_size_pt: _about_font_size_base

  // Padding/Margins that can be adjusted for rounded corners
  readonly property real screen_padding: (Qt.platform.os === 'android' ? 16 : 10)

  // TODO document better
  // This is for smartphone's and small screen, If they have a screen width less than this
  //   we adjust the layout for that kind of display
  readonly property int min_width_threshold: 700

  // Depending upon the language, we need to make tha bout text a bit bigger (maybe)
  readonly property real _about_font_size_base: {
    var is_japanese = Qt.locale().name.startsWith('ja');    // Most likely `ja_JP`, but I once got `ja_US`...
    var base = (is_japanese ? 15 : 12);
    // Scale up for Android as requested
    return (Qt.platform.os === 'android' ? base * 1.35 : base);
  }

  /** Checks to see if a given width is considered "small" for the theme */
  function is_width_small(width)
    { return (width < root.min_width_threshold); }
}
