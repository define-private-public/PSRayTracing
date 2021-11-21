import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import SixteenBPP
import SixteenBPP.Views


Window {
  id: root

  // TODO reset after done with dev
  minimumWidth: 350//640
  minimumHeight: 200//480
  width: 960
  height: 540
  visible: true
  title: Messages.app_window_title

  // This is neededed: On Android when the user pressed the Back button, Qml
  // will close the app.  Instead, we'd like it to go back to the previous view.
  // Only if that isn't possible, then close the app.
  onClosing: {
    if (stack_view.depth > 1) {
      close.accepted = false;
      stack_view.pop();
    } else {
      // If we're closing the window, first kill any active renders,
      //   Unfortunately the process could hang until a RenderTask/scanline has completed (this depends on the sample count per pixel)
      render_view.stop_active_render();
    }
  }

  // Controller for all of the views
  StackView {
    id: stack_view
    anchors.fill: parent
    initialItem: render_view
//    initialItem: about_view

    RenderView {
      id: render_view

      visible: false
      width: parent.width
      height: parent.height

      // If the about button is clicked, put on the "About" View
      onAbout_button_clicked: stack_view.push(about_view);
    }

    AboutView {
      id: about_view;

      visible: false
      width: parent.width
      height: parent.height

      // If the close button is clicked, pop this view (thus going back to the render view)
      onClose_button_clicked: stack_view.pop();
    }
  }
}
