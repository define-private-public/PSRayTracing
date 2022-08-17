import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import SixteenBPP
import SixteenBPP.Widgets


// This is a form were the user can adjust the settings for a render
Item {
  id: root

  // Property thats used for sizing/margins/layout
  QtObject {
    id: __p

    readonly property int padding_amount: 15
    readonly property int padding_amount_2x: 2 * __p.padding_amount

    readonly property int true_screen_width: g_renderer.screen_resolution.width
    readonly property int true_screen_height: g_renderer.screen_resolution.height

    /**
     * Figure out what the render size should be from the two files.
     */
    function set_render_size_property() {
      root.render_size.width = width_field.value_as_int();
      root.render_size.height = height_field.value_as_int();
    }

    /** Will load and set the render settings from the pesistant settings store */
    function load_render_settings_from_settings_store()
    {
      // Set the editor fields with the data from the settings store
      width_field.text = g_settings.render_size.width;
      height_field.text = g_settings.render_size.height;
      samples_per_pixel_field.text = g_settings.samples_per_pixel;
      max_depth_field.text = g_settings.max_depth;
      num_threads_field.text = g_settings.num_threads;
      seed_str_field.text = g_settings.seed_str;
      deep_copy_per_thread_field.checked = g_settings.deep_copy_per_thread;
    }
  }

  Component.onCompleted: {
    // Forward some signals
    ok_button.clicked.connect(root.ok_button_clicked);
    about_button.clicked.connect(root.about_button_clicked);

    __p.load_render_settings_from_settings_store();
  }

  // These are the rendering settings's values
  property size render_size:            '960x540'
  property int samples_per_pixel:       samples_per_pixel_field.value_as_int()
  property int max_depth:               max_depth_field.value_as_int()
  property int num_threads:             num_threads_field.value_as_int()
  property string seed_str:             seed_str_field.text
  property bool deep_copy_per_thread:   deep_copy_per_thread_field.checked

  signal ok_button_clicked()
  signal about_button_clicked()

  // Title of the form
  Text {
    id: title

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.margins: __p.padding_amount

    text: Messages.render_settings
    font.pointSize: UITheme.title_font_size_pt
  }

  ScrollView {

    anchors.margins: __p.padding_amount
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: title.bottom
    anchors.bottom: ok_button.top

    clip: true
    ScrollBar.horizontal.interactive: true
    ScrollBar.vertical.interactive: true

    Flickable {
      width: parent.width
      height: parent.height

      contentWidth: parent.width
      contentHeight: fields_layout.height


      // The actual area where the actual form is
      GridLayout {
        id: fields_layout

        // If the width is small...
        readonly property bool _small_width:           UITheme.is_width_small(parent.width)
        readonly property int column_count:            (_small_width ? 1 : 2)                           // we only want one column (label on top of field); else, use two (label left of field);
        readonly property real row_spacing_multiplier: (_small_width ? 0.5 : 1.5)                       // use less row spacing (makes labels closer to fields)
        readonly property real field_bottom_margin:    (_small_width ? (1.5 * __p.padding_amount) : 0)  // add some bottom margin to the fields (spaces fields better)

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: __p.padding_amount_2x

        columns: fields_layout.column_count
        rowSpacing: (fields_layout.row_spacing_multiplier * __p.padding_amount)
        columnSpacing: (2 * __p.padding_amount)

        Label {
          text: Messages.render_size

          // This is done so that the label aligns better with the column field below
          Layout.alignment: Qt.AlignTop
          Layout.topMargin: 10
        }
        ColumnLayout {
          Layout.minimumWidth: use_device_resolution_button.width
          Layout.maximumWidth: use_device_resolution_button.width
          Layout.bottomMargin: fields_layout.field_bottom_margin

          // These are the two integer fields to specify resolution
          RowLayout {
            id: render_size_edit

            Layout.fillWidth: true

            readonly property real __field_width: (use_device_resolution_button.width / 2) - (x_text.width) - 1

            IntegerOnlyTextField {
              id: width_field

              Layout.minimumWidth: render_size_edit.__field_width
              Layout.maximumWidth: render_size_edit.__field_width

              text: '960'
              placeholderText: Messages.width_min_value
              lower_bound: 1
              upper_bound: 1000000

              onTextChanged: __p.set_render_size_property();
              Keys.onReturnPressed: Qt.inputMethod.hide();
            }

            Text {
              id: x_text

              text: 'x'
            }

            IntegerOnlyTextField {
              id: height_field

              Layout.minimumWidth: render_size_edit.__field_width
              Layout.maximumWidth: render_size_edit.__field_width

              text: '540'
              placeholderText: Messages.height_min_value
              lower_bound: 1
              upper_bound: 1000000

              onTextChanged: __p.set_render_size_property();
              Keys.onReturnPressed: Qt.inputMethod.hide();
            }
          }

          // This button will set it to the detected screen resolution
          Button {
            id: use_device_resolution_button

            readonly property string __device_resolution_str: '%1x%2'.arg(__p.true_screen_width).arg(__p.true_screen_height);

            text: Messages.use_device_resolution_fmt.arg(__device_resolution_str)

            // This is done to make the layout less broken (on German)
            Layout.minimumWidth: max_depth_field.width
            Layout.maximumWidth: max_depth_field.width

            onClicked: {
              // Set the resolution for the fields
              width_field.text = __p.true_screen_width;
              height_field.text = __p.true_screen_height;
            }
          }
        }

        Label { text: Messages.samples_per_pixel }
        IntegerOnlyTextField {
          id: samples_per_pixel_field

          Layout.minimumWidth: use_device_resolution_button.width
          Layout.maximumWidth: use_device_resolution_button.width
          Layout.bottomMargin: fields_layout.field_bottom_margin

          text: '10'
          placeholderText: Messages.min_value_of_one
          lower_bound: 1
          upper_bound: 1000000

          Keys.onReturnPressed: Qt.inputMethod.hide();
        }

        Label { text: Messages.max_ray_depth }
        IntegerOnlyTextField {
          id: max_depth_field

          Layout.fillWidth: true
          Layout.bottomMargin: fields_layout.field_bottom_margin

          text: '50'
          placeholderText: Messages.min_value_of_one
          lower_bound: 1
          upper_bound: 10000

          Keys.onReturnPressed: Qt.inputMethod.hide();
        }

        // Upper bound for this field is set to 2x the supported as an experiment for the user
        Label {
          // This is done so that the label aligns better with the text field below
          Layout.alignment: Qt.AlignTop
          Layout.topMargin: 10

          text: Messages.number_of_threads
        }
        ColumnLayout {
          Layout.fillWidth: true
          Layout.bottomMargin: fields_layout.field_bottom_margin

          IntegerOnlyTextField {
            id: num_threads_field

            Layout.fillWidth: true

            text: '1'
            placeholderText: Messages.min_value_of_one
            lower_bound: 1
            upper_bound: (2 * g_renderer.num_concurrent_threads_supported())

            Keys.onReturnPressed: Qt.inputMethod.hide();
          }

          // This button will set it to the recommended amount
          Button {
            Layout.minimumWidth: use_device_resolution_button.width
            Layout.maximumWidth: use_device_resolution_button.width

            text: Messages.use_recommended_value_fmt.arg(g_renderer.num_concurrent_threads_supported())

            onClicked: { num_threads_field.text = g_renderer.num_concurrent_threads_supported(); }
          }
        }

        Label { text: Messages.random_seed }
        TextField {
          id: seed_str_field

          Layout.fillWidth: true
          Layout.bottomMargin: fields_layout.field_bottom_margin

          text: 'ASDF'

          Keys.onReturnPressed: Qt.inputMethod.hide();
        }

        Label { text: Messages.deep_copy_per_thread }
        Switch {
          id: deep_copy_per_thread_field

          Layout.alignment: Qt.AlignRight
          Layout.bottomMargin: fields_layout.field_bottom_margin

          checked: true
        }

        Label { text: "" }    // Empty spacer
        Button {
          // Button to reset the render settings back to their default values
          id: reset_to_default_button

          Layout.minimumWidth: use_device_resolution_button.width
          Layout.maximumWidth: use_device_resolution_button.width
          Layout.bottomMargin: fields_layout.field_bottom_margin

          text: Messages.reset_to_default

          onClicked: {
            // First reset on disk, then reload the form
            g_settings.reset_render_settings_to_default();
            __p.load_render_settings_from_settings_store();
          }
        }


        // Spacer for the "about" that's at the bottom
        Rectangle {
          Layout.bottomMargin: (3 * __p.padding_amount)
          Layout.fillWidth: true
          Layout.columnSpan: fields_layout.column_count
          height: (__p.padding_amount / 4)

          color: UITheme.render_display_background_color
        }


        // Button that launches an "About" page
        Button {
          id: about_button

          Layout.columnSpan: fields_layout.column_count
          Layout.fillWidth: true

          text: Messages.about_ps_raytracing
        }
      }
    }

  }



  // This button is used to confirm the input and dismiss the popup
  // It should not be clickable unless all input is valid
  Button {
    id: ok_button

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: __p.padding_amount

    text: (enabled ? Messages.ok : Messages.valid_input_please)

    // Make sure input is valid for the text input fields
    enabled: width_field.acceptableInput &&
             height_field.acceptableInput &&
             samples_per_pixel_field.acceptableInput &&
             max_depth_field.acceptableInput &&
             num_threads_field.acceptableInput &&
             seed_str_field.acceptableInput

    onClicked: {
      // When clicked, we will save fields to the settings store
      g_settings.render_size = render_settings_form.render_size;
      g_settings.samples_per_pixel = render_settings_form.samples_per_pixel;
      g_settings.max_depth = render_settings_form.max_depth;
      g_settings.num_threads = render_settings_form.num_threads;
      g_settings.seed_str = render_settings_form.seed_str;
      g_settings.deep_copy_per_thread = render_settings_form.deep_copy_per_thread;
    }
  }
}
