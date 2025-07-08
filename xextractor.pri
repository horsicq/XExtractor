INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

XCONFIG += use_dex
XCONFIG += use_pdf
XCONFIG += use_archive

HEADERS += \
    $$PWD/xextractor.h \
    $$PWD/xmodel_extractor.h

SOURCES += \
    $$PWD/xextractor.cpp \
    $$PWD/xmodel_extractor.cpp

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../Formats/xformats.pri)
}

!contains(XCONFIG, xmodel) {
    XCONFIG += xmodel
    include($$PWD/../Controls/xmodel.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xextractor.cmake
