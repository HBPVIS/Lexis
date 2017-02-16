# Copyright (c) HBP 2016-2017 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Lexis VERSION 1.2.0)
set(Lexis_VERSION_ABI 3)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
                              ${CMAKE_SOURCE_DIR}/CMake/common)
include(GitExternal)

set(LEXIS_DESCRIPTION
  "Vocabulary of events and shared objects for BBP and HBP software")
set(LEXIS_MAINTAINER "Human Brain Project <HBPVis@googlegroups.com>")
set(LEXIS_LICENSE LGPL)

set(COMMON_PROJECT_DOMAIN ch.epfl.bluebrain)
include(Common)
common_find_package(Boost COMPONENTS unit_test_framework)
common_find_package(Qt5Core)
common_find_package(vmmlib REQUIRED)
common_find_package(ZeroBuf REQUIRED)
common_find_package(ZeroEQ)
common_find_package_post()

set(LEXIS_DEPENDENT_LIBRARIES vmmlib ZeroBuf)
if(TARGET Qt5Core)
  list(APPEND LEXIS_DEPENDENT_LIBRARIES Qt5Core)
endif()

add_subdirectory(lexis)
add_subdirectory(apps)
add_subdirectory(tests)

set(DOXYGEN_MAINPAGE_MD README.md)
set(DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md)
include(CPackConfig)
include(DoxygenRule)
