# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

include(zerobufGenerateCxx)
set(__outdir ${PROJECT_BINARY_DIR}/include/lexis)

set(LEXIS_DIR ${__outdir})
set(LEXIS_FBS ${CMAKE_CURRENT_SOURCE_DIR}/request.fbs)
zerobuf_generate_cxx(LEXIS ${LEXIS_DIR} ${LEXIS_FBS})

set(LEXIS_DATA_DIR ${__outdir}/data)
set(LEXIS_DATA_FBS
  ${CMAKE_CURRENT_SOURCE_DIR}/data/cellSetBinaryOp.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/data/frameRange.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/data/selections.fbs
)
set(LEXIS_DATA_DETAIL_FBS ${CMAKE_CURRENT_SOURCE_DIR}/data/progress.fbs)
zerobuf_generate_cxx(LEXIS_DATA ${LEXIS_DATA_DIR} ${LEXIS_DATA_FBS})
zerobuf_generate_cxx(LEXIS_DATA_DETAIL ${LEXIS_DATA_DIR}/detail
  ${LEXIS_DATA_DETAIL_FBS})

set(LEXIS_RENDER_DIR ${__outdir}/render)
set(LEXIS_RENDER_FBS
  ${CMAKE_CURRENT_SOURCE_DIR}/render/materialLUT.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/exit.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/frame.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/imageJPEG.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/lookOut.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/lookupTable1D.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/stream.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/viewport.fbs
)
set(LEXIS_RENDER_DETAIL_FBS
  ${CMAKE_CURRENT_SOURCE_DIR}/render/clipPlanes.fbs
  ${CMAKE_CURRENT_SOURCE_DIR}/render/histogram.fbs
)
zerobuf_generate_cxx(LEXIS_RENDER ${LEXIS_RENDER_DIR} ${LEXIS_RENDER_FBS})
zerobuf_generate_cxx(LEXIS_RENDER_DETAIL ${LEXIS_RENDER_DIR}/detail
  ${LEXIS_RENDER_DETAIL_FBS})

set(LEXIS_PUBLIC_HEADERS
  ${LEXIS_HEADERS}
  ${LEXIS_DATA_HEADERS}
  ${LEXIS_DATA_DETAIL_HEADERS}
  ${LEXIS_RENDER_HEADERS}
  ${LEXIS_RENDER_DETAIL_HEADERS}
  data/Progress.h
  render/ClipPlanes.h
  render/Histogram.h
)

list(APPEND LEXIS_SOURCES
  ${LEXIS_DATA_SOURCES}
  ${LEXIS_DATA_DETAIL_SOURCES}
  ${LEXIS_RENDER_SOURCES}
  ${LEXIS_RENDER_DETAIL_SOURCES}
  data/Progress.cpp
  render/ClipPlanes.cpp
  render/Histogram.cpp
)

set(LEXIS_LINK_LIBRARIES PUBLIC vmmlib ZeroBuf)

common_library(Lexis)

if(TARGET Qt5::Core)
  add_subdirectory(qt)
endif()
