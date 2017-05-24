Lexis
=======

# Overview

Lexis is the vocabulary used for event-driven communication in and between
visualization software, mainly from the [BlueBrain](https://github.com/BlueBrain)
and [HBPVIS](https://github.com/HBPVIS) world. It provides various events and
shared objects which are implemented on top of
[ZeroBuf](https://github.com/HBPVIS/ZeroBuf).

# Building from Source

Lexis is a cross-platform library, designed to run on any modern operating
system, including all Unix variants. It requires a C++11 compiler and uses CMake
to create a platform-specific build environment. The following platforms and
build environments are tested:

* Linux: Ubuntu 16.04, RHEL 6.8 (Makefile, Ninja)
* Mac OS X: 10.9 (Makefile, Ninja)

Building from source is as simple as:

    git clone --recursive https://github.com/HBPVIS/Lexis.git
    mkdir Lexis/build
    cd Lexis/build
    cmake -GNinja -DCLONE_SUBPROJECTS=ON ..
    ninja
