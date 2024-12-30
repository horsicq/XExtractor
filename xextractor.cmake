include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFORMATS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)
    set(XEXTRACTOR_SOURCES ${XEXTRACTOR_SOURCES} ${XFORMATS_SOURCES})
endif()

set(XEXTRACTOR_SOURCES
    ${XEXTRACTOR_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xextractor.h
)
