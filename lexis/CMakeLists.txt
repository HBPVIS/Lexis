# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

include(zerobufGenerateCxx)
set(__outdir ${PROJECT_BINARY_DIR}/include/lexis)

set(LEXIS_DATA_DIR ${__outdir}/data)
set(LEXIS_DATA_FBS data/frameRange.fbs)
set(LEXIS_DATA_DETAIL_FBS data/progress.fbs)
zerobuf_generate_cxx(LEXIS_DATA ${LEXIS_DATA_DIR} ${LEXIS_DATA_FBS})
zerobuf_generate_cxx(LEXIS_DATA_DETAIL ${LEXIS_DATA_DIR}/detail
  ${LEXIS_DATA_DETAIL_FBS})

set(LEXIS_RENDER_DIR ${__outdir}/render)
set(LEXIS_RENDER_FBS
  render/exit.fbs
  render/frame.fbs
  render/imageJPEG.fbs
  render/lookOut.fbs
  render/lookupTable1D.fbs
)
zerobuf_generate_cxx(LEXIS_RENDER ${LEXIS_RENDER_DIR} ${LEXIS_RENDER_FBS})

set(LEXIS_PUBLIC_HEADERS
  ${LEXIS_DATA_HEADERS}
  ${LEXIS_DATA_DETAIL_HEADERS}
  ${LEXIS_RENDER_HEADERS}
  data/Progress.h
)

set(LEXIS_SOURCES
  ${LEXIS_DATA_SOURCES}
  ${LEXIS_DATA_DETAIL_SOURCES}
  ${LEXIS_RENDER_SOURCES}
  data/Progress.cpp
)

set(LEXIS_LINK_LIBRARIES PUBLIC ZeroBuf)

common_library(Lexis)

if(TARGET Qt5::Core)
  set(__outdir ${PROJECT_BINARY_DIR}/include/lexisqt)

  set(LEXISQT_DATA_DIR ${__outdir}/data)
  zerobuf_generate_qobject(LEXISQT_DATA ${LEXISQT_DATA_DIR} ${LEXIS_DATA_FBS})

  set(LEXISQT_RENDER_DIR ${__outdir}/render)
  zerobuf_generate_qobject(LEXISQT_RENDER ${LEXISQT_RENDER_DIR} ${LEXIS_RENDER_FBS})

  set(LEXISQT_PUBLIC_HEADERS
    ${LEXISQT_DATA_HEADERS}
    ${LEXISQT_RENDER_HEADERS}
  )

  set(LEXISQT_SOURCES
    ${LEXISQT_DATA_SOURCES}
    ${LEXISQT_RENDER_SOURCES}
  )

  set(LEXISQT_LINK_LIBRARIES PUBLIC Qt5::Core ZeroBuf)

  set(LEXISQT_INCLUDE_NAME lexisqt)
  set(LEXISQT_NAMESPACE lexisqt)
  common_library(LexisQt)
endif()
