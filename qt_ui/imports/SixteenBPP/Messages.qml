pragma Singleton

import QtQuick


// User facing strings
QtObject {
  readonly property string _render_library_version:   'r7'
  readonly property string _qt_ui_version:            '1.0'

  // Render View
  readonly property string app_window_title:        qsTr('PSRayTracing (GUI Edition)')
  readonly property string scene_label:             qsTr('Scene:')
  readonly property string stop:                    qsTr('Stop')
  readonly property string render:                  qsTr('Render')
  readonly property string total_render_time_fmt:   qsTr('Total Render Time: %1')

  // Used in the Render Settings Form
  readonly property string render_settings:             qsTr('Render Settings')
  readonly property string render_size:                 qsTr('Render Size')
  readonly property string width_min_value:             qsTr('Width (minimum value of 1)')
  readonly property string height_min_value:            qsTr('Height (minimum value of 1)')
  readonly property string use_device_resolution_fmt:   qsTr('Use Device Resolution: %1')
  readonly property string samples_per_pixel:           qsTr('Samples Per Pixel')
  readonly property string min_value_of_one:            qsTr('(minimum value of 1)')
  readonly property string max_ray_depth:               qsTr('Max Ray Depth')
  readonly property string number_of_threads:           qsTr('Number of Threads')
  readonly property string use_recommended_value_fmt:   qsTr('Use Recommended Value: %1')
  readonly property string random_seed:                 qsTr('Random Seed')
  readonly property string deep_copy_per_thread:        qsTr('Deep Copy per Thread')
  readonly property string ok:                          qsTr('OK')
  readonly property string valid_input_please:          qsTr('Please give valid input for each field above.')
  readonly property string about_ps_raytracing:         qsTr('About PSRayTracing')

  // Render Display
  readonly property string instructions:            qsTr('Press "Render" to start rendering.\nAdjust settings with the button on the lower left.')
  readonly property string render_in_progress_fmt:  qsTr('Rendering...\n%1\n%2\%')
  readonly property string stopping_active_render:  qsTr('Stopping Active Render...')

  // PanZoom Control
  readonly property string zoom_to_fit:  qsTr('Zoom to Fit')
  readonly property string zoom_to_100:  qsTr('Zoom to 100%')

  // About View
  readonly property string close:           qsTr('Close')
  readonly property string about_contents:  qsTr(
'<p>\
  PSRayTracing is an implementation of <a href="https://twitter.com/Peter_shirley">Peter Shirley\'s</a> <a href="https://raytracing.github.io/">Ray Racing in one\
  Weekend book series.</a>  While keeping the same architectural structure of the rendering logic it, is has alterations and improvements to its project layout,\
  algorithms, and portability; all while being done in vanilla C++ 17.  On average, it can render in 1/4 of the time as the reference code from the book.  It was\
  an exercise too in now the ordering of code can affect the compiled output assembly and the performance of a program.\  The CMake build system also features the\
  ability to toggle on (and off) the changes from the book\s code, as to see the performance impact.\
</p>\
<br>\
<p>\
  This app itself is a GUI frontend for the ray tracer.  Written in C++ &amp; Qt 6, it also acts as a way to (easily) test the performance of the ray tracer on\
  platforms such as Android and iOS.\  As of right now, only the scenes from books 1 &amp; 2 are available in this app.  Book 3\'s renders are supported but\
  (currently) cannot work alongside 1 &amp; 2\'s due to architectural issues.\
</p>\
<br>\
<p>\
  The project is open source with it\'s code freely available here:<br>\
  <a href="https://github.com/define-private-public/PSRayTracing">https://github.com/define-private-public/PSRayTracing</a>\
</p>\
<br>\
<p>\
  Read More:\
  <ul>\
    <li><a href="https://16bpp.net/blog/post/psraytracing-a-revisit-of-the-peter-shirley-minibooks-4-years-later/">PSRayTracing, A revisit of the Peter Shirley Minibooks 4 years later</a></li>\
    <li><a href="https://16bpp.net/blog/post/automated-testing-of-a-ray-tracer/">Automated Testing of a Ray Tracer</a></li>\
    <li><a href="https://16bpp.net/blog/post/making-a-cross-platform-mobile-desktop-app-with-qt-62/">Making A Cross Platform Mobile & Desktop App with Qt 6.2</a></li>\
    <li><a href="https://16bpp.net/blog/post/ray-tracing-book-series-review-nim-first-impressions/">Ray Tracing in Nim</a></li>\
  </ul>\
</p>\
<p>\
  Thanks:\
  <ul>\
    <li>Peter Shirley</li>\
    <li>Yining Karl Li</li>\
    <li><a href="https://github.com/define-private-public/PSRayTracing#thanks">&amp; more</a></li>\
  </ul>\
</p>\
')

  readonly property string info_contents: qsTr('
<p>\
  Info:\
  <ul>\
    <li>Render Library Version -- %1</li>\
    <li>Qt UI (App) Version -- %2</li>\
  </ul>\
</p>\
'
  ).arg(_render_library_version).arg(_qt_ui_version)

  // Multi-line strings feel a little silly in QML
}
