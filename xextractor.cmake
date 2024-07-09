include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)

set(XEXTRACTOR_SOURCES
    ${XFORMATS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.h
)
