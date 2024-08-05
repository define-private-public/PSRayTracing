#########################################
Peter Shirley Ray Tracing (in modern C++)
#########################################

|book2_final_render|


The official place where I do most of the work for this is `over on GitLab <https://gitlab.com/define-private-public/PSRayTracing>`_
Though I do field questions on the `GitHub mirror <https://github.com/define-private-public/PSRayTracing/>`_
To contact me, you can always @ me `on Twitter as @DefPriPub <https://twitter.com/DefPriPub>`_  (`Or my personal website <https://16bpp.net/>`_)

Now with an Android, iOS, and macOS (Apple Silicon) GUI:

|PSRayTracingGooglePlay|_
|PSRayTracingAppleAppStore|_

(It also runs on Windows, Mac (Intel), Linux,, but you need to build that for yourself in the meantime.)


.. contents ::


*************
What Is This?
*************

This repo is an implementation of Peter Shirley's `Ray Tracing mini-books`_, done in modern C++ (17),
with a different project layout and all sorts of performance boosts.  The CMake build script allows
the user to toggle on/off most of the changes made from the reference implementation; as to better
see the effects of these changes.

Compared to the book's code, mine is able to render the final scene in about 1/4 of the time.  Without
radically changing the architecture of the renderer and using (near) vanilla C++.

There are other fun features such as multi-core rendering, configurable render parameters (e.g. samples
per pixel, max ray depth), and progress bars; it's the little things that count.  It should be able to
replicate all of the scenes from all three books.

What I like about how this book is structured is that it shows you how to make incremental changes to
the ray tracer.  With these step-by-step changes in book 3 though, it was getting very difficult to
have my code reproduce every step, due to many structural changes in critical sections.  Originally the
code for book 3 was off in its own separate branch (called ``book3``), but it has now been mereged into
the mainline branch (that is ``master``).  The ``PDFVariant`` optimization has also come along with it
(and is now a CMake configuration variable instead of a second separate branch).  Note that getting book 3's
code to work alongside that of books 1 & 2 did required adjusting some of the architecture a bit; but not
my too much.

I've decided to license this implementation under the ``Apache License (2.0)``.  The full text is in the
file ``LICENSE.txt``.  The only exception to this is ``src/third_party``, as those are externally provided
projects.  Check those files for their licenses.  Note that the books (and the book code) is under
``CC0 1.0 Universal``.  If you end up using any of this, a shout out would be appreciated (and telling me
too).



****
Why?
****

I first went through the books back in mid 2016 when they were new.  That time I did it though as `an exercise to learn Nim`_
It's kind of cool to see how much these books have blown up in popularity.  Ray Tracing seems to be
quite in vogue recently, and I was looking for a project to learn modern C++ and better project architecture.
This was also an experiment to see how far I could push a CPU render and optimize my code while keeping
things simple, portable, and reproducible.


***************
Some Blog Posts
***************

Care to read a little more about this project?  Check out these:

* `Making the first few revisions of the project <https://16bpp.net/blog/post/psraytracing-a-revisit-of-the-peter-shirley-minibooks-4-years-later/>`_

* `Using Python to do automated testing & performance measuring <https://16bpp.net/blog/post/automated-testing-of-a-ray-tracer/>`_

* `Adding a Qt/QML UI that works seamlessly on Mobile & Desktop <https://16bpp.net/blog/post/making-a-cross-platform-mobile-desktop-app-with-qt-62/>`_

* `Localizing the GUI app into Japanese (日本語）and German (Deutsch) <https://16bpp.net/blog/post/localizing-a-qt-app-or-anything-else-for-that-matter/>`_

  * This one is more about localizing Qt apps & software localization in general; not so much ray tracing

* `Measuring the impact of the final keyword <https://16bpp.net/blog/post/the-performance-impact-of-cpp-final-keyword/>`_

* `Finding out when noexcept helps performance (and when it doesn't) <https://16bpp.net/blog/post/noexcept-can-sometimes-help-or-hurt-performance/>`_




************************
Building and Basic Usage
************************

I initially developed this on an Ubuntu 18.04 machine using GCC 10.x; but now its moved to newer versions
of Ubuntu (22.04 LTS) and GCC (11 & 12).  It runs on Windows 10/11 via MSYS2 (also GCC).  It also compiles
via clang 11+ without a hitch.  macOS & xcode/clang have also been testing and verified. Something I could
use help with is getting a build working with MSVC on Windows.


====================================
Qt/QML based UI for Mobile & Desktop
====================================

Don't want to use the command line interface?  Want to see how this performs on your phone, tablet, or chromebook?
Take a look at the ``qt_ui/`` subfolder.  Be sure the check the ``README.rst`` over there for some more instructions.


============
Requirements
============

* A C++17 compiler.  I'd recommend GCC 11/12 (or higher)

* CMake 3.x.  Using a CMake GUI application (such as ``ccmake``) will make your life easier if you want
  to toggle on/off changes from the reference implementation


============
How To Build
============

1. In the root of this project, make a build directory and go into it: ``mkdir build && cd build/``

2. Set your desired compiler (e.g. ``export CC=gcc-12``, ``export CXX=g++-12``)

3. Run CMake w/ build type set to be a release: ``cmake ../ -DCMAKE_BUILD_TYPE=Release``

4. Build it: ``make``



**********
How To Use
**********

If you ever get lost, doing a simple ``./PSRayTracing --help`` should give you a list of all the options
available, as well as their defaults.  Here are some of the more important ones:

* ``--scene <scene_id_string>``, scene from book(s) to render (default is ``book2::final_scene``).

* ``-n <integer>``, number of samples per pixel (default is ``25``)

* ``-j <integer>``, how many cores/threads to render with (default is ``1``)

* ``-s <integer>x<integer>``, resolution of the render (default is ``960x540``)

* ``-o <filename>.png``, file to save the render to (default is ``render.png``).  Note that this will always
  overwrite any existing file.

If you want to see what scenes are available to render, supply ``--list-scenes`` as an argument and it should
show you all that are implemented.  They should be in order as they were presented in the book.

So for example, If you do ``./PSRayTracing`` by itself, it will render Book 2's final scene, with 25 samples
per pixel, on a single core, saving it to ``render.png`` with a resolution of 540p.  Though if you do
``./PSRayTracing -n 250 -j 4 -s 1920x1080 --scene book1::normal_sphere``, you'll get that sphere using it's
surface normal (to shade it), with 250 samples/pixel, rendering on four cores at 1080p; also saving to
``render.png``.

You should be good now to start rendering.

If you want to see what scenes, supply the flag ``--list-scenes`` to the executable.  The output form this is
different depending upong what branch you're currently on.



*****************
What's Different?
*****************

Oh, quite a bit.  This isn't an exhaustive list though.  (I forgot to document some stuff along the way,
*my bad ¯\\_( ツ )_/¯*)


========================
Structural/Architectural
========================

1. Naming changes; e.g. I did things like ``BVHNode`` instead of ``bvh_node``.  These are just preferences
   of mine

2. Classes like ``material`` have become strict abstract base classes, ``IMaterial`` for instance; take a
   look at ``src/Interfaces/`` to see more

3. Instead of rendering to PPM files (via standard output), ``stb_image_write`` is used to write directly
   to PNGs; A file format that you're not ashamed to take back home and introduce to Mom.

4. The ``random_*`` functions have put into their own ``RandomGenerator`` object; this helps perf. and
   multi-threading (more further down).

5. More files/structs/classes and compartmentalization of functionality.  Imma neat freak

6. A lot more ``const`` and ``const ref`` parameters, these help perf. and create cleaner, more predictable
   code

7. More/less inline functions

   * Sometimes inlining makes sense and can boost perf well (e.g. ``Vec3``). For virtual functions (.e.g
     ``IMaterial``, ``IHittable``, etc), it does jack

8. Using ``Vec3`` as a Point

   * *Yeah yeah*, I can hear one of my CG professors from college saying *“A point is not a vector, a vector
     is not a point.  You can't add or multiply two points.”* But I wanted to keep things easy here.  ``Vec3``,
     3D points, and colors are used quite interchangeably in this code

9. ``Vec3`` is also not backed by an array of three elements.  We have a hard ``x``, a hard ``y``, and a
   hard ``z``.  Saved me typing parenthesis

10. The ``Box`` type also requires you to pass in an RNG to it's constructor.  More on why in the perf. section

11. Avoiding defining our own Deconstructors and copy/move/assignment constructors.

    * Modern compilers are really nice.  Sometimes they do this for you.  It's less code we have to write,
      which also means less bugs

12. ``ImageTexture``'s method of object creation is different; it's constructor has a different signature.
    And you are instead recommed to use a "static constructor" method instead.  One which loads an image from
    a filepath, the other which loads an image from a memory buffer.

    * This was done to make Android & iOS port easier to develop.  There is only a single image used in the
      entire program (``assets/earthmap.jpg``).  So embedding this makes life much easier instead of having
      to worry about how those platforms look for runtime assets.  And if you want to add your own image
      textures into a scene (directly from a file), I've left the older loading function in, commented out
      (see ``Scenes/Book2.cpp``).

13. The ray tracer has been split into two parts, a static library (where the rendering code lives) and a
    "CLI Runner".  This was done so I could make it easier to build the Qt UI.  This is an architectural
    change that was introduced after the ``r7`` release.

14. To get all three book's to be able to render with the same branch, I needed to add an enumeration called
    ``RenderMethod``.  This is passed into some of the ``IMaterial`` methods.  Books 1 & 2 have the same exact
    method of perfoming renders, but book 3 is different in its "rendering equation" (e.g. the use of ``PDF``s).
    It doesn't clutter up the code/architecture too much and after testing, it didn't hamper performance whatsover.


===========
Render-wise
===========

Not too much.  The final scene for book 2 replicates the perlin noise texture of the book's cover, not what's
actually in the book code.  A ``SurfaceNormal`` material was also added so one of the first generated images can
be included for completeness.


================
Performance-wise
================

This is the juicy good stuff that you're here for.

Using ``CMAKE_BUILD_TYPE=Release``, rendering the final scene (with same samples/pixel, resolution, single
threaded of course), my implementation would compute the result in about 1/4 the time it took the book's code.
This implementation supports multiple cores/threads, so it can render the final image even faster.

I do want to note that the doubling the amount of core/threads rendering doesn't give me that ideal 50%
reduction in render time.  E.g. ``1 core = 120 sec``, ``2 cores = 72 sec``, ``4 cores = 43 sec``, etc..).
This was my first time implementing a thread pool in C++.  If someone knows how to improve on my multi-threading
code, please send a PR my way.  I think there is some pointer chasing going on that is hampering perf. too.

My code is structured differently, so it's very likely there were perf. benefits from that as well.  If you
look at the CMake build file (``src/CMakeLists.txt``), you should see that there are a lot of added ``ON/OFF``
options, that all begin with ``WITH_*`` (e.g. ``WITH_BOOK_PERLIN``, ``WITH_BOOK_SQRT``, etc).  These are
changes that differ from the book code.  I made them toggleable on/off so you can better see the effects they
have on render time (some even have effects on the final image).  Using a CMake GUI application (like ``ccmake``)
it is pretty easy to do all this toggling.

Some of them created massive perf. boosts for me (e.g. 12%, 7%, etc.).  While some others were very tiny, it
was hard to measure (e.g. 1%, 0.5%); which could be within the margin of error.  So I will say that not all
of these are fully conclusive, but I would like others to take a look as well and report to me if my method
or the book's method was better on your system.  For instance, I'm on an ``Intel(R) Core(TM) i5-7300U CPU @
2.60GHz``.  (I'd like to see what one of those magical AMD Ryzen Threadripper does. :] )

I'm not going to go into gory details of the code, as it can get a bit long.  If you' interested in seeing
what I did for a certain option, search through the code (C++) looking for ``#ifdef`` statements that have
a corresponding ``USE_*`` to them.  For example, if you wanted to look to see what I wrote for
``WITH_BOOK_AABB_HIT``, search for ``USE_BOOK_AABB_HIT``.



*******************************
The Changes That Made An Impact
*******************************

======================================================
Rewriting Code for SIMD and Trying to Reduce Branching
======================================================

There were many parts of the code and can be rewritten and moved around with having the same computed result,
but computed in a more efficient manner.  I'm finding it a little hard to correctly explain how this works,
so I think it would be best to go in and look at the code differences.  In some places, I actually had the
result of an ``if`` body computed right before that ``if``, hoping that the compiler would reorder instructions
and pack computations together via auto-vectorization.  This is what I did in a lot of ``*::hit()`` functions.

Branching (i.e. ``if`` statements for you non-assembly people) can be a real performance killer.  Only do
branching if you need to for the correctness of an algorithm, or to save time on an expensive computation
(e.g. checking a cache).

I think the best change to show off for this is my implementation of the ``AABB::hit()`` function.  It went
from a bunch of ``if`` s, ``swap()`` s, and value comparisons computed sequentially, to a very parallelizable
batch of ``min()`` s and ``max()`` es.

I would like to make a note about branch prediction.  I'm not sure if I was able to successfully exploit it
in my implementation (see my ``ray_color()`` function).  If you have the time, reading this `Stack Overflow
post <https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array>`_
It provides some great insight into what's going on at the CPU level to make your code faster.

The people who write compilers and create CPUs are the smartest in the world.  The best we can do is write
our programs to utilize their genius.

============================
Trigonometric Approximations
============================

Trig functions are necessary for almost anything math.  Though, they can also get a little expensive to
compute.  But in some cases, such as graphics, we can get away with doing a faster approximation of the
functions.  In our case, we use the functions ``sin()``, ``cos()``, ``asin()``, and ``atan2()``.

Sine, cosine, and arcsine use a taylor series approximation.  It's fairly easy enough to implement.
``atan2()`` as a bit more tricky and required bitwidling magic.  My method was taken from
`this page <https://www.dsprelated.com/showarticle/1052.php>`_.  I'd really recommend reading through it if
you want to know the details of how it worked.

Keep in mind that since these are approximations, they're going to differ from the ground truth.  Here's a
series of images that explain it better.

|asin_ground_truth|

|asin_approx_no_ec|

|asin_approx_with_ec|

If it's hard for you to see the difference between the first and third renders, load them up in an image
viewer and toggle between them really fast.

I used to have a math professor scoff at approximations.  I mean, they are technically incorrect.  But in our
program we express Pi as ``3.1415926535``; that's an approximation, not the actual value of Pi.  If we're doing
that, any approximation is fair game to use as long as the viewer has no idea it's different.


=====================
Building a Better Box
=====================

In the books' code, the ``Box`` object is actually made up of six components.  Two ``XYRect``, two ``XZRect``,
and two ``YZRect``.  Using a ``HittableList`` to store them all, and then loop through it for the ``hit()``
detection.  While this is pretty simple, it can be done better.

Take for example this stress test.  It is a 5x5x5 matrix of glass cubes in a Cornell Box.  Using the book's
method on my machine,  it took around 8 minutes and 28 seconds (528 sec); 600x600 with 100 samples per pixel
(single threaded).  Its scene id is ``fun::cornell_glass_boxes``

|cornell_glass_boxes|

---------------------------
Using BVHNode (2nd Fastest)
---------------------------

Now, in a prior chapter, we actually made a ``BVHNode`` object.  Having ``Box`` actually use that to store the
rectangles (and perform the ``hit()``) was much faster.  The only complication from this is now construction of
a ``Box`` object requires an RNG, but it's really a small price to pay.

With the use of the ``BVHNode``, this dropped down to 6 minutes and 5 seconds (365 sec).  That's already a speedup
of ~30%.

  I've removed this implementation of ``Box`` from the current revision because the method mentioned below is more
  performant.  If you wish to see this one though, checkout commit tagged ``r1``.

-------------------------------------------
Writing a Proper ``hit()`` Method (Fastest)
-------------------------------------------

To push this even further, it's better to give the Box object its own ``hit()`` implementation, rather than relying
on that of the ``*Rect`` children.  To do this we simply take the code for each individual ``*Rect::hit()`` function,
place it in ``Box::hit()``, and then rewrite it to take better advantage of SIMD instructions.  This way the ray-side
hit intersection is being computed in parallel.  And since we don't need to store a list of pointers to more objects,
this also helps us trim down on memory usage.

This reduced the render time of the "Cornell Glass Boxes" down to 5 minutes and 34 seconds (334 sec).  That's an extra
~10% upon the BVHNode method, but ~40% on the book's method!


=========================
PCG Random & a RNG Object
=========================

When doing random number generation, you're not limited to what's provided out of the box in C++.  As a
replacement for the Mersenne twister engine from the standard, `PCG <https://www.pcg-random.org>`_ provides a
drop in RNG that is better performing.

On top of that, the book's RNG solution was to use essentially one source for generation; a bunch of functions
prefixed with ``random_*``.  This created issues when I was trying to multi-thread the ray tracer (initially
with OpenMP).

|bad_rng_black_speckles|

Black speckles were showing up on the multi-core enabled renders.  In a Twitter thread someone suggested that
I make sure the RNG is thread safe, which it wasn't.  My implementation creates a branch new RNG object per
scanline (each seeded from a master RNG).  It fixed this issue and improved performance.


====================
Deep Copy per Thread
====================

The list of objects to render (or tree, whatever term you prefer), if you notice, is actually a collection
of ``std::shared_ptr<T>`` types.  In some cases, using a shared pointer can make a lot of sense.  For example
if two different objects have the same texture or material.  If you update the material on one of the objects,
you'll see the change on the other.  A common complaint of shared pointers is that they are slow (e.g. because
of reference counting).  This can even further cause issues when you throw multi-core access into the mix.
Luckily, the rendering process is read-only when it comes to the scene, so we don't need to worry about any of
those multi-threaded reading/writing/access issues you hear so much about.

I started with the hypothesis: *"Copying the scene (to render) to each thread/core would improve the render
performance."*  I created the ``IDeepCopyable`` interface, and then implemented it on every class that could be
rendered (e.g. Spheres, Textures, Boxes, etc.).  It requires that you add a function called ``copy()``, which
must return a deep copy of the object and of its child objects.  Then when setuping up the render threads, in
each one, ``copy()`` is called on the root of the scene.

So for single core rendering, there's no improvement.  But when rendering with multiple cores, I saw signifcant
improvements.  Sometimes in the range of it being 20-30% faster! I also want to note that different scenes did
not have all the same benefits.  To be honest, I'm not fully sure why this is the case.  My assumption is because
multiple threads are not having to fight over a single shared pointer tree.   I did ask Reddit's ``/r/cpp_questions/``
why it could be faster.  If you would like to read the thread `it's right over here <https://www.reddit.com/r/cpp_questions/comments/jl4vdd/why_exactly_did_copying_a_tree_of_pointers_to/>`_ .
Or if you may know why this is more performant, please tell me so I can put that information here.

A better thing to do in my opinion is not to rely on shared pointers unless you really need to.

By default the "deep copy per thread" feature is on, but it can be toggled off at runtime by supplying the
``--no-copy-per-thread`` flag to the executable.


===========================================================
Using a raw pointer over shared pointers (in ``HitRecord``)
===========================================================

Going off from the above section, it would be good to eliminate (or reduce) the use of ``std::shared_ptr<T>`` in
our code.

Going off on a little aside here, part of my inspiration to revisit this book series was watching `Tyler Morgan-Wall <https://twitter.com/tylermorganwall>`_
build his `Rayrender/Rayshader project <https://github.com/tylermorganwall/rayrender/>`_ (a path tracer for R).
Like this one, it's also based off of the Peter Shirley books.  `In a recent tweet <https://twitter.com/tylermorganwall/status/1342328063510638593>`_
he announced that he was able improve render speeds by about 20%.  `After poking through his commit history <https://github.com/tylermorganwall/rayrender/commit/7358edf19e0ccf4478a8f9975f4c418e15841783>`_,
there is a change in the ``HitRecord`` structure.  ``mat_ptr`` was changed from being a
``std::shared_ptr<IMaterial>`` over to a plain old raw pointer (``IMaterial *``).

Making that change to this ray tracing project also led to a good speed boost.  When testing this out on an older
Gen 7 i5 CPU, I got a render speed improvement of about 30%!  On a Gen 10 i7 CPU, the speed boost was closer to 10%;
not as grand, but still quite significant.

Since ``HitRecord`` is used a lot, including the ``mat_ptr`` field, it makes a lot of sense here to remove the shared
pointer usage.  We have no intention of modifying the material used, only to know what it is.  And during the render
process, none of the objects or materials will change.  This is a perfect place to use a raw pointer.

If you want to try toggling this on/off, this is controlled by the ``WITH_BOOK_MAT_PTR`` flag at CMake configuration
time.


===================================
PDFVariant (For Book 3 Scenes Only)
===================================

While working on Book 3, I couldn't help but notice that during the rendering process, we were allocating dynamic
memory and creating shared pointers when it came to using classes like ``CosinePDF``, ``HittablePDF``, and
``MixturePDF`` (all subclasses of ``IPDF``).  These classes weren't being used in any extraordinary complex ways.
For instance ``CosinePDF`` is only being used in ``IMaterial`` objects.  ``HittablePDF`` is only being used with
the light objects for a scene.  And ``MixturePDF`` is only instantiated in the ``ray_color()`` function.

Leveraging ``std::variant<T>``, type we can still pass around these various ``IPDF`` sublcasses in a flexable manor,
but ensure that they stay allocated on the stack (thus no dynamic memory or reference counting).  This was shoved
into an aliased type called ``PDFVariant``.  We still need to work with pointers to ``IPDF`` (namely for
``MixturePDF``), but these are much faster raw pointers (and the memory actually lives on the stack).



*****************
Other Experiments
*****************

Not everything is a success.  I had some theories that I wanted to test that turned out to fail.


===============================
Single Floating Point Precision
===============================

If you look through the code, you'll find there's next to no mention of ``float`` or ``double`` directly.  It
has this type called ``rreal``.  That's actually an alias to one of those two; it defaults to ``double``.
I was wondering if using less precision would be more performant (since it doesn't have to use as many bytes
in memory).  Turns out that wasn't the case for most of the development; it was exactly the same.

Though later on (I can't remember where/when), ``float`` started to perform worse than ``double``.  I haven't
figured out what the cause of it was.  It's truly a bit perplexing to me.  If anyone might know why, I'm all
ears.

  As an aside, I used to use ``real`` as the data type, but I later found out that ``std::complex<T>`` has a
  member function called ``real()``.  Luckily humans were smart enough to invent refactoring tools.


=========================
Square Root Approximation
=========================

Chasing after `that famous fast inverse square root  approximation (of Quake 3 frame)
<https://en.wikipedia.org/wiki/Fast_inverse_square_root>`_, I did some research in square root approximation.
`This article was an interesting read. <https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi>`_
I tried the Babylonian method without much success.  I did learn quite a bit about the history of computing
square roots.  For me, in the end it turned out those methods were slower and more incorrect.

``std::sqrt()`` is king.


=================================
Marking functions as ``noexcept``
=================================

**Update Aug 5th, 2024:** After doing an experiment with the ``final`` keyword and writing a blog post on it,
someone reached out and asked me if I had done the same but with ``noexcept``.  Now a more comprehensive
performance test of the ``noexcept`` keyword has been done.  The results are mixed and you can `read it right here <https://16bpp.net/blog/post/noexcept-can-sometimes-help-or-hurt-performance/>`_ .
Orginally I left ``noexcept`` on, but now I've decided to turn it off.

I remember hearing about how marking my functions with the `noexcept <https://visualstudiomagazine.com/articles/2016/10/01/noexcept.aspx>`_
could make my code run faster.  So to test this out (and test toggling it on & off), I added a macro called
``NOEXCEPT``, which will either expand to the ``noexcept`` keyword, or will be a null string.  As a benchmark
I did single core renders with the sample-per-pixels set to 250.

To my surprise, I found out that there was no signficant change in render time with ``noexcept`` on the functions
or not.  Each one took about 230 seconds in total, with a difference of about ~0.5 seconds (which can be
attributed to error).  My code is very much exception sparse, so I don't think it would have helped that much
anyways.  Other code bases may benefit from this, but this one definately did not.

Despite having no real performance benefits (at least for me), I still think adding ``noexcept`` is a good
C++ practice; especially for APIs.  It informs other developers of your design intentions.  *If you write code
that could crash someone else's, it's best to tell others that it could happen*.  C++ documetation tools such
as Doxygen will pick up those ``noexcept`` keywords and mark it in the generated docs.


==================
BVH Tree as a List
==================

The idea here is that I thought the ``BVHNode`` object was a little inefficient when it came to memory usage.  It required
that you create have two ``IHittable`` objects as children (which could also be ``BVHNodes``).  Instead, the BVH tree could
be a list AABBs, that also contained indices to child AABBs.  But maybe some of those indices actually pointed to objects
that could be hit and produce colour.  It practically became "pointers but with a lot more steps involved". I don't want
to go into the gory details of how it works.  If you want to see, look at the code for the class ``BVHNode_MorePerformant``.

This one had a much more minor speedup.  On some newer hardware, I only saw about a 1-2% performance boost.  When it came
an older machine, it was more in the range of 5-9% (which is more on the significant side).  This one didn't seem to be
as significant as other changes.  Not to mention it was hard to reproduce results that saw a constant performance boost
accross different hardware.  It is on by default though; it can be toggled on/off via a CMake configuration variable.

The tree construction and hit algorithms are the same as the book's BVH node (depth first).  It's very likely that
alterative construction and hit algorithms could produce more performant results.



*********************************
Looking Back and Thinking Forward
*********************************

Overall this was a fun project.

I'd love to visit some of these ideas, as they could bring better perf and add all around fun features, but
I want to get onto other projects.  Someday...

* Being able to pause and restart renders.  Should be simple, but I'd want to do it

* Adding in a scripting language to define scenes (instead of hard coding them in).

  * I think `ChaiScript <https://chaiscript.com>`_ would be a good candidate for this.  Fancy animations
    would also be more possible then!

* There should be some ways to lay out the memory and objects differently to gain more perf.
  ``std::shared_ptr<>`` isn't a zero cost abstraction.  Reducing the amount of pointers (and dynamically
  allocated memory) can really boost performance.  Though, I think a more radically different structure is
  required for this renderer

* CPUs and AVX instructions are fun and all, but let's not kid ourselves, GPUs are the alpha dogs in this realm.
  If I knew CUDA, OpenCL, or Vulkan better this renderer could very likely be in a real time state.

* Techniques such as adaptive sampling would be a boon.  But I wanted to keep this repo strictly to topics
  that were mentioned in the book

* Adding in some more fun features like metaballs or an
  `“0ldskool” plasma effect <https://www.bidouille.org/prog/plasma>`_.  Let's be real here, it isn't a true
  CG application unless you support these.  LAN party like it's '96.

I will be visiting Ray Tracing again sometime in the future.


******
Thanks
******

We're all working off of the works of others, in some way or another.  Let me highlight those that had a
bit more of an impact on this project.

* `Peter Shirley <https://twitter.com/Peter_shirley>`_, he wrote this book series initially

* `Matt Godbolt <https://twitter.com/mattgodbolt>`_ for his compiler explorer tool.  It has been invaluable
  when trying to play code golf for generated assembly and seeing if things get vectorized.  It's a must use
  for anyone who doing performance tweaking in C++

* `Nic Taylor <https://twitter.com/NicTVG>`_ for his ``atan2()`` approximation

* `Roman Wiche (a.k.a. Bromanz) <https://twitter.com/romanwiche>`_ for his Ray-AABB intersection article
  (and code)

* `Tyler Morgan-Wall <https://twitter.com/tylermorganwall>`_ for working on `Rayrender/Rayshader <https://github.com/tylermorganwall/rayrender/>`_
  which provided me with inspiration to start this project.  As well providing another hint on how to boost
  render speed

* The folks over on Reddit's C++ community answering my questions (`/r/cpp <https://reddit.com/r/cpp>`_ and
  `/r/cpp_questions <https://reddit.com/r/cpp_questions>`_)

* Those who work on the Boost, PCG Random, cxxopts (jarro2783), and stb libraries





.. |book2_final_render| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/book2_final_n10000.png
  :width: 1920
  :height: 1080
  :scale: 50

.. |bad_rng_black_speckles| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/black_spots_on_render.png
.. |asin_ground_truth| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_ground_truth.png
.. |asin_approx_no_ec| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_approx_no_ec.png
.. |asin_approx_with_ec| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_approx_with_ec.png
.. |cornell_glass_boxes| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/cornell_glass_boxes.png
.. |PSRayTracingGooglePlay| image:: https://play.google.com/intl/en_us/badges/static/images/badges/en_badge_web_generic.png
.. |PSRayTracingAppleAppStore| image:: https://tools.applemediaservices.com/api/badges/download-on-the-app-store/black/en-us

.. _`Ray Tracing mini-books`: https://raytracing.github.io
.. _`an exercise to learn Nim`: https://16bpp.net/blog/post/ray-tracing-book-series-review-nim-first-impressions/
.. _PSRayTracingGooglePlay: https://play.google.com/store/apps/details?id=net.sixteenbpp.psraytracing&amp;pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1
.. _PSRayTracingAppleAppStore: https://apps.apple.com/us/app/psraytracing/id6468944135
