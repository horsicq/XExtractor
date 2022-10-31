INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

XCONFIG += use_dex
XCONFIG += use_pdf
XCONFIG += use_archive

HEADERS += \
    $$PWD/xextractor.h

SOURCES += \
    $$PWD/xextractor.cpp

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../Formats/xformats.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xextractor.cmake
