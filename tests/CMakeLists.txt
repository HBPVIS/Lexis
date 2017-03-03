# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

# Change this number when adding tests to force a CMake run: 2

if(NOT BOOST_FOUND)
  return()
endif()

set(TEST_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} Lexis)
include(CommonCTest)
