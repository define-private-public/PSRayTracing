import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SixteenBPP
import SixteenBPP.Widgets


Rectangle {
  id: root
  state: 'startup'

  color: UITheme.render_display_background_color

  signal render_double_tapped

  QtObject {
    id: __p

    function show_render()
    {
      // First need to clear out the image then load it again
      render_image.image_source = ''
      render_image.image_source = 'file:///' + g_renderer.render_file_path;
    }
  }

  onStateChanged: {
    // If the state becomes "complete", then we have a render to show
    if (state == 'show_render')
      __p.show_render();
  }


  StackLayout {
    id: stack_layout

    anchors.fill: parent

    currentIndex: 0

    // Text message
    Item {
      width: parent.width
      height: parent.height

      Text {
        id: display_msg

        anchors.centerIn: parent

        text: Messages.instructions
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14
      }
    }

    // The display for the rendered image (in a PanZoom control)
    PanZoom {
      id: render_image
      visible: false

      // Make our images appear more "pixelly" when zoomed in
      // TODO does this effect it when the image is too large? (e.g. zooming to 50%, does this look bad)
      image_smooth: false

      // Required to work (see docs for `PanZoom`)
      width: parent.width
      height: parent.height

      // Bubble up the double tap signal?
      onDouble_tapped: {
        // Bubble up the signal & toggle the pan/zoom functionality
        root.render_double_tapped();
        render_image.pan_zoom_enabled = !render_image.pan_zoom_enabled;
      }
    }
  }

  states: [
    // Show instructions
    State {
      name: 'startup'
      when: (g_renderer.status == PSRayTracingRenderer.Status.Idle)
      PropertyChanges { target: stack_layout;  currentIndex: 0 }
      PropertyChanges { target: display_msg;  text: Messages.instructions }
    },

    // Render in progress
    State {
      name: 'render_in_progress'
      when: (g_renderer.status == PSRayTracingRenderer.Status.InProgress)
      PropertyChanges { target: stack_layout;  currentIndex: 0 }
      PropertyChanges {
        target: display_msg
        text: Messages.render_in_progress_fmt.arg(g_renderer.render_time_str).arg(g_renderer.render_progress_str)
      }
    },

    State {
      name: 'stopping_render'
      when: (g_renderer.status == PSRayTracingRenderer.Status.StoppingRender)
      PropertyChanges { target: stack_layout;  currentIndex: 0 }
      PropertyChanges { target: display_msg;   text: Messages.stopping_active_render }
    },

    // Show completed render
    State {
      name: 'show_render'
      when: (g_renderer.status == PSRayTracingRenderer.Status.Complete)
      PropertyChanges { target: stack_layout;  currentIndex: 1 }
    }
  ]
}
