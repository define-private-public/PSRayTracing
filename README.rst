#####################################
Peter Shirley Ray Tracing (in C++ 17)
#####################################

|book2_final_render|


The official place where I do most of the work for this is `over on GitLab <https://gitlab.com/define-private-public/PSRayTracing>`_
Though I do field questions on the `GitHub mirror <https://github.com/define-private-public/PSRayTracing/>`_
To contact me, you can always @ me `on Twitter as @DefPriPub <https://twitter.com/DefPriPub>`_  (`Or my personal website <https://16bpp.net/>`_)


.. contents ::


*************
What Is This?
*************

This repo is an implementation of Peter Shirley’s `Ray Tracing mini-books`_, done in modern C++ (17),
with a different project layout and all sorts of performance boosts.  The CMake build script allows
the user to toggle on/off most of the changes made from the reference implementation; as to better
see the effects of these changes.

Compared to the book’s code, mine is able to render the final scene in about 1/4 of the time.  Without
radically changing the architecture of the renderer and using (near) vanilla C++.

There are other fun features such as multi-core rendering, configurable render parameters (e.g. samples
per pixel, max ray depth), and progress bars; it’s the little things that count.  It should be able to
replicate all of the scenes from books 1 & 2.  Though there is no book 3 implementation at this time.

What I like about how this book is structured is that it shows you how to make incremental changes to
the ray tracer.  With these step-by-step changes in book 3 though, it was getting very difficult to
have my code reproduce every step, due to many structural changes in critical sections.  So I decided
it would be best to keep book 3 as its own separate thing for a later date.  But on top of that, I
think there are more points of optimization to talk about for Books 1 & 2.



****
Why?
****

I first went through the books back in mid 2016 when they were new.  That time I did it though as `an exercise to learn Nim`_
It’s kind of cool to see how much these books have blown up in popularity.  Ray Tracing seems to be
quite in vogue recently, and I was looking for a project to learn C++17.  This was also an experiment
to see how far I could push a CPU render and optimize my code while keeping things simple, portable,
and reproducible.


************************
Building and Basic Usage
************************

I developed this on an Ubuntu 18.04 machine using GCC 10.x.  I was able to get it to run under Windows
10 via MSYS2 (also GCC).  It also compiles under clang 11 without a hitch.  I haven’t tested it on OS X,
but I’m sure it will build just fine there.  Something I could use help with is getting a build working
with MSVC on Windows.


============
Requirements
============

* A C++17 compiler.  I’d recommend GCC 10 or higher

* CMake 3.x.  Using a CMake GUI application (such as ``ccmake``) will make your life easier if you want
  to toggle on/off changes from the reference implementation

* Boost.  I’m using their argument parsing library.

  * If someone knows a good replacement that doesn’t change the command line interface, please tell me.
    I’m not using any other parts of Boost and I’d like to remove this dependency

    * That is, unless someone points out a way I can leverage Boost for more perf. bumps


============
How To Build
============

1. In the root of this project, make a build directory and go into it: ``mkdir build && cd build/``

2. Set your desired compiler (e.g. ``export CC=gcc-10``, ``export CXX=g++-10``)

3. Run CMake w/ build type set to be a release: ``cmake ../src/ -DCMAKE_BUILD_TYPE=Release``

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

So for example, If you do ``./PSRayTracing`` by itself, it will render Book 2’s final scene, with 25 samples
per pixel, on a single core, saving it to ``render.png`` with a resolution of 540p.  Though if you do
``./PSRayTracing -n 250 -j 4 -s 1920x1080 --scene book1::normal_sphere``, you’ll get that sphere using it’s
surface normal (to shade it), with 250 samples/pixel, rendering on four cores at 1080p; also saving to
``render.png``.

You should be good now to start rendering.



*****************
What’s Different?
*****************

Oh, quite a bit.  This isn’t an exhaustive list though.  (I forgot to document some stuff along the way,
*my bad ¯\\_( ツ )_/¯*)


========================
Structural/Architectural
========================

1. Naming changes; e.g. I did things like ``BVHNode`` instead of ``bvh_node``.  These are just preferences
   of mine

2. Classes like ``material`` have become strict abstract base classes, ``IMaterial`` for instance; take a
   look at ``src/Interfaces/`` to see more

3. Instead of rendering to PPM files (via standard output), ``stb_image_write`` is used to write directly
   to PNGs; A file format that you’re not ashamed to take back home and introduce to Mom.

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
     is not a point.  You can’t add or multiply two points.”* But I wanted to keep things easy here.  ``Vec3``,
     3D points, and colors are used quite interchangeably in this code

9. ``Vec3`` is also not backed by an array of three elements.  We have a hard ``x``, a hard ``y``, and a
   hard ``z``.  Saved me typing parenthesis

10. The ``Box`` type also requires you to pass in an RNG to it’s constructor.  More on why in the perf. section

11. Avoiding defining our own Deconstructors and copy/move/assignment constructors.

    * Modern compilers are really nice.  Sometimes they do this for you.  It’s less code we have to write,
      which also means less bugs


===========
Render-wise
===========

Not too much.  The final scene for book 2 replicates the perlin noise texture of the book’s cover, not what’s
actually in the book code.  A ``SurfaceNormal`` material was also added so one of the first generated images can
be included for completeness.


================
Performance-wise
================

This is the juicy good stuff that you’re here for.

Using ``CMAKE_BUILD_TYPE=Release``, rendering the final scene (with same samples/pixel, resolution, single
threaded of course), my implementation would compute the result in about 1/4 the time it took the book’s code.
This implementation supports multiple cores/threads, so it can render the final image even faster.

I do want to note that the doubling the amount of core/threads rendering doesn’t give me that ideal 50%
reduction in render time.  E.g. ``1 core = 120 sec``, ``2 cores = 72 sec``, ``4 cores = 43 sec``, etc..).
This was my first time implementing a thread pool in C++.  If someone knows how to improve on my multi-threading
code, please send a PR my way.  I think there is some pointer chasing going on that is hampering perf. too.

My code is structured differently, so it’s very likely there were perf. benefits from that as well.  If you
look at the CMake build file (``src/CMakeLists.txt``), you should see that there are a lot of added ``ON/OFF``
options, that all begin with ``WITH_*`` (e.g. ``WITH_BOOK_PERLIN``, ``WITH_BOOK_SQRT``, etc).  These are
changes that differ from the book code.  I made them toggleable on/off so you can better see the effects they
have on render time (some even have effects on the final image).  Using a CMake GUI application (like ``ccmake``)
it is pretty easy to do all this toggling.

Some of them created massive perf. boosts for me (e.g. 12%, 7%, etc.).  While some others were very tiny, it
was hard to measure (e.g. 1%, 0.5%); which could be within the margin of error.  So I will say that not all
of these are fully conclusive, but I would like others to take a look as well and report to me if my method
or the book’s method was better on your system.  For instance, I’m on an ``Intel(R) Core(TM) i5-7300U CPU @
2.60GHz``.  (I’d like to see what one of those magical AMD Ryzen Threadripper does. :] )

I’m not going to go into gory details of the code, as it can get a bit long.  If you’ interested in seeing
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
but computed in a more efficient manner.  I’m finding it a little hard to correctly explain how this works,
so I think it would be best to go in and look at the code differences.  In some places, I actually had the
result of an ``if`` body computed right before that ``if``, hoping that the compiler would reorder instructions
and pack computations together via auto-vectorization.

Branching (i.e. ``if`` statements for you non-assembly people) can be a real performance killer.  Only do
branching if you need to for the correctness of an algorithm, or to save time on an expensive computation
(e.g. checking a cache).

I think the best change to show off for this is my implementation of the ``AABB::hit()`` function.  It went
from a bunch of ``if`` s, ``swap()`` s, and value comparisons computed sequentially, to a very parallelizable
batch of ``min()`` s and ``max()`` es.

I would like to make a note about branch prediction.  I’m not sure if I was able to successfully exploit it
in my implementation (see my ``ray_color()`` function).  If you have the time, reading this `Stack Overflow
post <https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array>`_
It provides some great insight into what’s going on at the CPU level to make your code faster.

The people who write compilers and create CPUs are the smartest in the world.  The best we can do is write
our programs to utilize their genius.

============================
Trigonometric Approximations
============================

Trig functions are necessary for almost anything math.  Though, they can also get a little expensive to
compute.  But in some cases, such as graphics, we can get away with doing a faster approximation of the
functions.  In our case, we use the functions ``sin()``, ``cos()``, ``asin()``, and ``atan2()``.

Sine, cosine, and arcsine use a taylor series approximation.  It’s fairly easy enough to implement.
``atan2()`` as a bit more tricky and required bitwidling magic.  My method was taken from
`this page <https://www.dsprelated.com/showarticle/1052.php>`_.  I’d really recommend reading through it if
you want to know the details of how it worked.

Keep in mind that since these are approximations, they’re going to differ from the ground truth.  Here’s a
series of images that explain it better.

|asin_ground_truth|

|asin_approx_no_ec|

|asin_approx_with_ec|

If it’s hard for you to see the difference between the first and third renders, load them up in an image
viewer and toggle between them really fast.

I used to have a math professor scoff at approximations.  I mean, they are technically incorrect.  But in our
program we express Pi as ``3.1415926535``; that’s an approximation, not the actual value of Pi.  If we’re doing
that, any approximation is fair game to use as long as the viewer has no idea it’s different.


======================
Using BVHNode In a Box
======================

In this ray tracer, the ``Box`` object is actually made up of six others.  Two ``XYRect`` , two ``XZRect`` ,
and two ``YZRect``.  Using a ``HittableList`` to store them all, and then loop through it for the ``hit()``
detection.

Now, in a prior chapter, we actually made a ``BVHNode`` object.  Having ``Box` actually use that to
store the rectangles (and perform the ``hit()``) was much faster.  The only complication from this is now
construction of a ``Box`` object requires an RNG, but it’s really a small price to pay.

To push this even further, it should be possible to take out all of the ``hit()`` code for the rectangles, and
then put it inside of ``Box::hit()`` itself, then do some code rewriting to further reduce the time it takes
to compute a hit.  This would also have the added benefits of reducing the memory requirements for the ``Box``
object.  I didn’t implement this since it would probably take a significant amount of time and I wanted to move
onto other parts of the project.  I’ll leave this up as an exercise to anyone who wants to submit a PR.

===========================
PCG Random & and RNG Object
===========================

When doing random number generation, you’re not limited to what’s provided out of the box in C++.  As a
replacement for the Mersenne twister engine from the standard, `PCG <https://www.pcg-random.org>`_ provides a
drop in RNG that is better performing.

On top of that, the book’s RNG solution was to use essentially one source for generation; a bunch of functions
prefixed with ``random_*``.  This created issues when I was trying to multi-thread the ray tracer (initially
with OpenMP).

|bad_rng_black_speckles|

Black speckles were showing up on the multi-core enabled renders.  In a Twitter thread someone suggested that
I make sure the RNG is thread safe, which it wasn’t.  My implementation creates a branch new RNG object per
scanline (each seeded from a master RNG).  It fixed this issue and improved performance.



*****************
Other Experiments
*****************

Not everything is a success.  I had some theories that I wanted to test that turned out to fail.


===============================
Single Floating Point Precision
===============================

If you look through the code, you’ll find there’s next to no mention of ``float`` or ``double`` directly.  It
has this type called ``rreal``.  That’s actually an alias to one of those two; it defaults to ``double``.
I was wondering if using less precision would be more performant (since it doesn’t have to use as many bytes
in memory).  Turns out that wasn’t the case for most of the development; it was exactly the same.

Though later on (I can’t remember where/when), ``float`` started to perform worse than ``double``.  I haven’t
figured out what the cause of it was.  It’s truly a bit perplexing to me.  If anyone might know why, I’m all
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



*********************************
Looking Back and Thinking Forward
*********************************

Overall this was a fun project.

I’d love to visit some of these ideas, as they could bring better perf and add all around fun features, but
I want to get onto other projects.  Someday...

* An implementation of Book 3.  As stated before, it was getting to be a real pain to make sure every scene
  was reproducible.  Having Book 3 in here could make this feel more complete, but too many critical things
  kept on changing

* Being able to pause and restart renders.  Should be simple, but I’d want to do it

* Adding in a scripting language to define scenes (instead of hard coding them in).

  * I think `ChaiScript <https://chaiscript.com>`_ would be a good candidate for this.  Fancy animations
    would also be more possible then!

* There should be some ways to lay out the memory and objects differently to gain more perf.
  ``std::shared_ptr<>`` isn’t a zero cost abstraction.  Reducing the amount of pointers (and dynamically
  allocated memory) can really boost performance.  Though, I think a more radically different structure is
  required for this renderer

* CPUs and AVX instructions are fun and all, but let’s not kid ourselves, GPUs are the alpha dogs in this realm.
  If I knew CUDA, OpenCL, or Vulkan better this renderer could very likely be in a real time state.

* Techniques such as adaptive sampling would be a boon.  But I wanted to keep this repo strictly to topics
  that were mentioned in the book

* Adding in some more fun features like metaballs or an
  `“0ldskool” plasma effect <https://www.bidouille.org/prog/plasma>`_.  Let’s be real here, it isn’t a true
  CG application unless you support these.  LAN party like it’s ‘96.

* I’d like to add in a script that runs all possible configurations/permutations of the render, then compares
  it against a known ground truth.  The key to discovering better perf is through measuring and testing

I will be visiting Ray Tracing again sometime in the future.


******
Thanks
******

We’re all working off of the works of others, in some way or another.  Let me highlight those that had a
bit more of an impact on this project.

* `Peter Shirley <https://twitter.com/Peter_shirley>`_, he wrote this book series initially

* `Matt Godbolt <https://twitter.com/mattgodbolt>`_ for his compiler explorer tool.  It has been invaluable
  when trying to play code golf for generated assembly and seeing if things get vectorized.  It’s a must use
  for anyone who doing performance tweaking in C++

* `Nic Taylor <https://twitter.com/NicTVG>`_ for his ``atan2()`` approximation

* `Roman Wiche (a.k.a. Bromanz) <https://twitter.com/romanwiche>`_ for his Ray-AABB intersection article
  (and code) 

* The folks over on Reddit’s C++ community answering my questions (`/r/cpp <https://reddit.com/r/cpp>`_ and
  `/r/cpp_questions <https://reddit.com/r/cpp_questions>`_)

* Those who work on the Boost, PCG Random, and stb libraries





.. |book2_final_render| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/book2_final_n10000.png
  :width: 1920
  :height: 1080
  :scale: 50

.. |bad_rng_black_speckles| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/black_spots_on_render.png
.. |asin_ground_truth| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_ground_truth.png
.. |asin_approx_no_ec| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_approx_no_ec.png
.. |asin_approx_with_ec| image:: https://gitlab.com/define-private-public/PSRayTracing/-/raw/master/images/asin_approx_with_ec.png

.. _`Ray Tracing mini-books`: https://raytracing.github.io
.. _`an exercise to learn Nim`: https://16bpp.net/blog/post/ray-tracing-book-series-review-nim-first-impressions/

