include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFORMATS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)
    set(XEXTRACTOR_SOURCES ${XEXTRACTOR_SOURCES} ${XFORMATS_SOURCES})
endif()
if (NOT DEFINED XOPTIONS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../XOptions/xoptions.cmake)
    set(XEXTRACTOR_SOURCES ${XEXTRACTOR_SOURCES} ${XOPTIONS_SOURCES})
endif()
if (NOT DEFINED XMODEL_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xmodel.cmake)
    set(XEXTRACTOR_SOURCES ${XEXTRACTOR_SOURCES} ${XMODEL_SOURCES})
endif()

set(XEXTRACTOR_SOURCES
    ${XEXTRACTOR_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.h
    ${CMAKE_CURRENT_LIST_DIR}/xmodel_extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xmodel_extractor.h
)
