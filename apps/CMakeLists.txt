# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

if(TARGET ZeroEQ)
  set(LEXIS-PROGRESSMONITOR_SOURCES progressMonitor.cpp)
  set(LEXIS-PROGRESSMONITOR_LINK_LIBRARIES Lexis ZeroEQ)
  common_application(lexis-progressmonitor)

  set(LEXIS-SENDEVENT_SOURCES sendevent.cpp)
  set(LEXIS-SENDEVENT_LINK_LIBRARIES Lexis ZeroEQ)
  common_application(lexis-sendevent)
endif()
