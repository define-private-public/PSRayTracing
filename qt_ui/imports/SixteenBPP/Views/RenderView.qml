import QtQuick
import QtQuick.Controls
import SixteenBPP
import SixteenBPP.Widgets

// This is also the "main view" of the application.  It's where we can perform a render,
// adjust settings, and go into other views from
Item {
  id: root

  signal about_button_clicked()

  state: 'normal'
  property bool in_fullscreen_mode: false

  Component.onCompleted: {
    // Forward the clicking of the "About" button
    render_settings_form.about_button_clicked.connect(root.about_button_clicked);
  }

  // The rendeer result (that's displayed)
  RenderDisplay {
    id: render_display

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    onRender_double_tapped: root.handle_render_double_tapped();
  }

  // The bar that's bellow at the bottom
  ControlsBar {
    id: controls_bar

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 10

    onOpen_render_settings_clicked: render_settings_popup.open()

    onRender_button_clicked: {
      if (g_renderer.in_progress())
        root.stop_active_render();
      else
        root.start_render();
    }
  }

  // This popup start hidden off screen to transition in
  Drawer {
    id: render_settings_popup

    readonly property real padding_amount: 25

    width: parent.width
    height: root.height - (2 * render_settings_popup.padding_amount)

    // We want this popup to be modal.
    // The user must press the "OK" button (int the content item) to close it
    modal: true
    focus: true
    interactive: false
    closePolicy: Popup.NoAutoClose
    edge: Qt.TopEdge

    // This is the actual form that shows up.  It has an "OK" button that prevents the user
    // from closing the popup until all input has been validated.
    contentItem: RenderSettingsForm {
      id: render_settings_form

      anchors.centerIn: parent

      onOk_button_clicked: render_settings_popup.close();
      onAbout_button_clicked: render_settings_popup.close();
    }
  }

  states: [
    // normal state, show control bar and render/main area
    State {
      name: 'normal'
      when: !root.in_fullscreen_mode

      PropertyChanges { target: controls_bar;    visible: true; }
      AnchorChanges   { target: render_display;  anchors.bottom: controls_bar.top; }
    },

    // State for when the user wants the render fullscreen'd
    //   No control bar, render is front and centered
    //   (and hopefully zoomed in w/ pan controls)
    State {
      name: 'fullscreen_render'
      when: root.in_fullscreen_mode

      PropertyChanges { target: controls_bar;    visible: false; }
      AnchorChanges   { target: render_display;  anchors.bottom: parent.bottom; }
    }
  ]


  function start_render() {
    // Setup the render job
    g_renderer.start_render(
      render_settings_form.render_size,
      controls_bar.selected_scene_id,
      render_settings_form.samples_per_pixel,
      render_settings_form.max_depth,
      render_settings_form.num_threads,
      render_settings_form.seed_str,
      render_settings_form.deep_copy_per_thread
    );
  }


  function stop_active_render() {
    // Only do this if we're rendering (force stop)
    if (g_renderer.in_progress())
      g_renderer.stop_active_render();
  }


  function handle_render_double_tapped() {
    // If a render has completed, then double tapping is enabled, which means the image can be fullscreen'd
    root.in_fullscreen_mode = !root.in_fullscreen_mode;
  }

  // Function to call showing the render settings popup
  function show_render_settings()
    { render_settings_popup.open(); }
}

