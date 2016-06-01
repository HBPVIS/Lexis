# Copyright (c) HBP 2016 Daniel.Nachbaur@epfl.ch
# All rights reserved. Do not distribute without further notice.

set(__outdir ${PROJECT_BINARY_DIR}/include/lexisqt)

set(LEXISQT_DIR ${__outdir})
zerobuf_generate_qobject(LEXISQT ${LEXISQT_DIR} ${LEXIS_FBS})

set(LEXISQT_DATA_DIR ${__outdir}/data)
zerobuf_generate_qobject(LEXISQT_DATA ${LEXISQT_DATA_DIR} ${LEXIS_DATA_FBS})

set(LEXISQT_RENDER_DIR ${__outdir}/render)
zerobuf_generate_qobject(LEXISQT_RENDER ${LEXISQT_RENDER_DIR} ${LEXIS_RENDER_FBS})

set(LEXISQT_PUBLIC_HEADERS
  ${LEXISQT_HEADERS}
  ${LEXISQT_DATA_HEADERS}
  ${LEXISQT_RENDER_HEADERS}
)

list(APPEND LEXISQT_SOURCES
  ${LEXISQT_SOURCES}
  ${LEXISQT_DATA_SOURCES}
  ${LEXISQT_RENDER_SOURCES}
)

set(LEXISQT_LINK_LIBRARIES PUBLIC Qt5::Core ZeroBuf)

set(LEXISQT_INCLUDE_NAME lexisqt)
set(LEXISQT_NAMESPACE lexisqt)
common_library(LexisQt)
