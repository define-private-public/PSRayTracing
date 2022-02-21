##########################
Qt/QML Mobile & Dekstop UI
##########################

For anyone who wants to play around with this Ray Tracing implementation, it is a bit of a pain.  You need to clone
the repo, have CMake & a C++ compiler installed and know your way around a terminal.  I thought it would be nicer to
have a GUI application that could be readily downloaded either from GitLab/GitHub or from an app store.  So it was done,
using Qt.

This supports Windows, macOS, and Linux for Desktop.  Android & iOS are supported for mobile/tablet platforms.  It is
currently available in Google Play.  It's not on the Apple App store, but will be eventually.

|PSRayTracingGooglePlay|_




************
Requirements
************

It's pretty much what you need for the project above + Qt.

* A C++17 compiler.  I’d recommend GCC 10 or higher

* CMake 3.x.  Using a CMake GUI application (such as ``ccmake``) will make your life easier if you want
  to toggle on/off changes from the reference implementation

* Qt 6.2.x.  This project started out with 6.2.1, used 6.2.2, and is known to work with 6.2.3.  Later versions of Qt
  Should work.  If not, then please file a ticket on the [issue tracker](https://gitlab.com/define-private-public/PSRayTracing/-/issues).

  * If you're compiling for Android, make sure you install the Android tools/libraries from the Qt Maintenance. You'll
    also need to install the Android NDK and other tools.  The Qt Creator IDE does a good job of making this *very*
    easy.

  * If you're compiling for iOs, don't forget to have XCode installed.  IIRC, there might be some other "Qt for iOS"
    components you need to install with the Qt Maintenance Tool.



*********
Compiling
*********

I'd recommend using Qt Creator to do this, as it's very easy.  Though it can be done on the command line if you know
what you're doing.

You have to use the ``CMakeLists.txt`` file that's at the top of this repo (not the one in this folder).  The CMake
variable that controls Qt UI is built (or not) is aptly called ``BUILD_QT_UI``.  By default, it is turned ``OFF``,
though if you are building for Android or iOS, it wil automatically be set to ``ON`` at the CMake configuration time.
This was done to make the process a bit more streamlined.


=======
Desktop
=======

If you use the ``cmake`` command line utility (to build for Desktop), you'll need to specify ``-DBUILD_QT_UI=True``.
This is what I do for Linux.  But you can use the Qt Creator IDE (this is what I do for Windows & macOS).

If working with the Qt Creator IDE, at the CMake configuration step, find the ``BUILD_QT_UI`` and set it to being
checked.  Then reconfigure.  After that, you should be fine to build.

This will also build the CLIRunner as well.  So if using Qt Creator, when pressing the green "Run" arrow button, it
may launch the CLI app instead.  But don't worry, look in the build folder for `PSRayTracing_QtUI`.  That should be
the executable.


=============
Mobile/Tablet
=============

It's possible to use the command line for this, but I really recommend using Qt Creator; as it's only clicking buttons.
Make sure you've selected "Qt for Android" or "Qt for iOS" as the kit to use.  Note that iOS, there should be two
options "device" and "simulator".  Let it configure; the ``BUILD_QT_UI`` should automatically be turned ``ON`` when one
of these kits is detected.

* For Android, you should be good to build.  Once you're done with that and connected a valid device (with developer
  mode enabled).  The ``Build -> Deply`` option or pressing "Run" should be good.  It might take a while the first time.

* For iOS, in the build folder, open up the generated `.xcodeproj` file.  There should be multiple targets to build.
  Be sure to select the one called "PSRayTracing_QtUI", as it should have the app's icon right next to it.  Don't forget
  to specify the target device (e.g. "iPad Pro 5th Gen", "iPhone 11", "iOS simulator", etc).

  Once it's built, you should be okay to press Xcode's "Run" button.  Your iDevice might ask you to authorize the app.
  It's honestly a bit of a pain.  But then it should launch (slowly).  Note too that unlike Android, you can't really
  keep" the app on the device.  After 24 hours (I think), the app will be disabled and you'll need to put it again to
  the device.



.. |PSRayTracingGooglePlay| image:: https://play.google.com/intl/en_us/badges/static/images/badges/en_badge_web_generic.png

.. _PSRayTracingGooglePlay: https://play.google.com/store/apps/details?id=net.sixteenbpp.psraytracing&amp;pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1
