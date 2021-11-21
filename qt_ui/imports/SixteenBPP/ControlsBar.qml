import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SixteenBPP
import SixteenBPP.Widgets

// These are the controls that appear at the bottom of the app and are used to
// control the start/stop of the rendering as well as the settings. This widget
// will also display some information too.
Item {
  id: root
  state: 'startup'

  // The height is grabbed from a special internal layout property
  height: internal_layout.height_for_root

  property string selected_scene_id: scene_select_dropdown.currentText

  signal open_render_settings_clicked
  signal render_button_clicked

  QtObject {
    id: __p

    readonly property real padding: 25

    // This is a function that's called upon startup to set the iniali scene
    // that is selected in the dropdown
    function set_initial_selected_scene() {
      var i = scene_select_dropdown.find('book2::final_scene', Qt.MatchExactly);
      scene_select_dropdown.currentIndex = i;
    }
  }

  Component.onCompleted: __p.set_initial_selected_scene();

  // The internal widget/item where the widgets actually live
  //   Note that this is adaptive and has its own internal state
  Item {
    id: internal_layout
    state: 'normal_width'

    anchors.fill: parent

    // These are used for determinine the height for this layout
    //   It actually needs to bubble up to the main container (yes, I know this is a tad bit odd)
    readonly property real height_for_root: (state == 'normal_width') ? _normal_height : _small_width_height
    readonly property real _normal_height: 40
    readonly property real _small_width_height: 2 * _normal_height

    // Settings button to change, well, the render settings
    ToolButton {
      id: settings_button

      anchors.left: parent.left
      anchors.bottom: parent.bottom

      icon.source: UITheme.settings_icon

      onClicked: root.open_render_settings_clicked()
    }

    Label {
      id: scene_label

      anchors.left: settings_button.right
      anchors.verticalCenter: scene_select_dropdown.verticalCenter

      text: Messages.scene_label
    }

    // Drop down the select the scene
    ComboBox {
      id: scene_select_dropdown

      width: 330
      anchors.leftMargin: __p.padding
      anchors.left: scene_label.right
      anchors.top: parent.top

      model: g_renderer.available_scene_ids();
    }

    // This is a progress bar when rendering, but total time string when done
    //   Note: for better organization, this could be in its own .qml file, but I don't want to overengineer too much
    StackLayout {
      id: render_info_layout

      anchors.leftMargin: __p.padding
      anchors.rightMargin: __p.padding
      anchors.right: render_button.left
      anchors.verticalCenter: render_button.verticalCenter

      currentIndex: -1

      // Blank empty for startup
      Item { }

      // Shows render progress
      ProgressBar {
        id: progress_bar

        value: g_renderer.render_progress
      }

      // Message that shows reneder time (after render completes)
      Label {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: Messages.total_render_time_fmt.arg(g_renderer.render_time_str)
      }
    }

    // Button that starts/stops rendering
    Button {
      id: render_button

      anchors.right: parent.right
      anchors.bottom: parent.bottom

      text: Messages.render
      icon.source: UITheme.start_icon

      onClicked: root.render_button_clicked()
    }

    // State for controlling the widgets' layout
    states: [
      // For normal screen widths
      //   Layout goes left to right: settings button, scene label, scene select, status message, render button
      State {
        name: 'normal_width'
        when: !UITheme.is_width_small(root.width);

        AnchorChanges { target: scene_label;            anchors.left:  settings_button.right; }
        AnchorChanges { target: scene_select_dropdown;  anchors.right: undefined; }
        AnchorChanges { target: settings_button;        anchors.top:   parent.top; }
        AnchorChanges { target: render_info_layout;     anchors.left:  scene_select_dropdown.right; }
        AnchorChanges { target: render_button;          anchors.top:   parent.top; }

        PropertyChanges { target: scene_label;            anchors.leftMargin: __p.padding; }
        PropertyChanges { target: scene_select_dropdown;  width: 330; }
      },

      // When the width is too tiny
      //   Scene select (and label) goes on top
      //   Render settings, status message, and render goes on bottom
      State {
        name: 'small_width'
        when: UITheme.is_width_small(root.width);

        AnchorChanges { target: scene_label;            anchors.left:  parent.left; }
        AnchorChanges { target: scene_select_dropdown;  anchors.right: parent.right; }
        AnchorChanges { target: settings_button;        anchors.top:   scene_select_dropdown.bottom; }
        AnchorChanges { target: render_info_layout;     anchors.left:  settings_button.right; }
        AnchorChanges { target: render_button;          anchors.top:   scene_select_dropdown.bottom; }

        PropertyChanges { target: scene_label;            anchors.leftMargin: undefined; }
        PropertyChanges { target: scene_select_dropdown;  width: undefined; }
      }
    ]
  }

  // State for controlling what the widgets look like
  states: [
    // Show nothing
    State {
      name: 'startup'
      when: (g_renderer.status == PSRayTracingRenderer.Status.Idle)

      PropertyChanges { target: render_info_layout;     currentIndex: 0; }
      PropertyChanges { target: settings_button;        enabled: true; }
      PropertyChanges { target: scene_select_dropdown;  enabled: true; }
      PropertyChanges {
        target:       render_button
        enabled:      true
        text:         Messages.render
        icon.source:  UITheme.start_icon
      }
    },

    // Render in progress
    State {
      name: 'render_in_progress'
      when: (g_renderer.status == PSRayTracingRenderer.Status.InProgress)

      PropertyChanges { target: render_info_layout;     currentIndex: 1; }
      PropertyChanges { target: settings_button;        enabled: false; }
      PropertyChanges { target: scene_select_dropdown;  enabled: false; }
      PropertyChanges {
        target:       render_button
        enabled:      true
        text:         Messages.stop
        icon.source:  UITheme.stop_icon
      }
    },

    State {
      name: 'stopping_render'
      when: (g_renderer.status == PSRayTracingRenderer.Status.StoppingRender)

      PropertyChanges { target: render_info_layout;     currentIndex: 0; }
      PropertyChanges { target: settings_button;        enabled: false; }
      PropertyChanges { target: scene_select_dropdown;  enabled: false; }
      PropertyChanges {
        target:       render_button
        enabled:      false
        text:         Messages.stop
        icon.source:  UITheme.stop_icon
      }
    },

    // Show info about completed render
    State {
      name: 'show_render'
      when: (g_renderer.status == PSRayTracingRenderer.Status.Complete)

      PropertyChanges { target: render_info_layout;     currentIndex: 2; }
      PropertyChanges { target: settings_button;        enabled: true; }
      PropertyChanges { target: scene_select_dropdown;  enabled: true; }
      PropertyChanges {
        target:       render_button
        enabled:      true
        text:         Messages.render
        icon.source:  UITheme.start_icon
      }
    }
  ]
}
