About
=====

libmtkw is a general purpose lightweighted profiler.

Main targets of this profiler are server side applications.
Restarting server side applications is usually difficult.
Because this profiler does not require restarting of target processes,
it is very useful in emergency.

TODO: Write more!

How to install
==============

Requirements
------------

Boost C++ Libraries (libboost_thread-mt.so) must be installed.

Build and install
-------------------

$ ./autogen.sh
$ ./configure
$ make
$ make install # or $ sudo make install

How to use
==========

Writing now...

Sample code
-----------

::

  #include <mtkw/profiler.hpp>

  int g() {
    MTKW_SCOPED_PROFILE();
    // This scope is profiled
  }

  int f() {
    MTKW_PROFILE() { // This block is profiled
      g();
      g();
    }
  }

  int main() {
    mtkw::initialize();
    mtkw::enable(true);

    MTKW_PROFILE_N("start") { // with explicit name
      f();
    }

    // call mtkw::getLastProfile() to get profile
  }

``mtkw::getLastProfile`` return the ``Profile`` of the last profiled block.
``struct Profile`` has profiled data with hierarchies.
This sample code generate the information like below::

  start took XXX seconds
    f took YYY seconds
      g took ZZZ seconds
      g took ZZZ' seconds

XXX includes sum of YYY, ZZZ and ZZZ'.

You can dig down subprofiles via Profile::subprofiles.
In this case, Profile of "start" have a subprofile of f (subprofile[0]).