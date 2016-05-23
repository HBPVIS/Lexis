# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

if(TARGET ZeroEQ)
  set(PROGRESSMONITOR_SOURCES progressMonitor.cpp)
  set(PROGRESSMONITOR_LINK_LIBRARIES Lexis ZeroEQ)
  common_application(progressMonitor)
endif()
