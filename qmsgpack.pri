QT       += core

INCLUDEPATH += $$PWD/src

SOURCES += \
    $$PWD/src/msgpack.cpp \
    $$PWD/src/msgpackcommon.cpp \
    $$PWD/src/private/pack_p.cpp \
    $$PWD/src/private/unpack_p.cpp \
    $$PWD/src/msgpackstream.cpp

HEADERS += \
    $$PWD/src/msgpack.h \
    $$PWD/src/private/pack_p.h \
    $$PWD/src/private/unpack_p.h \
    $$PWD/src/endianhelper.h \
    $$PWD/src/msgpackcommon.h \
    $$PWD/src/msgpack_export.h \
    $$PWD/src/msgpackstream.h

!contains(DEFINES, MSGPACK_NO_PACKTYPES) {
    SOURCES += \
        $$PWD/src/private/qt_types_p.cpp \
        $$PWD/src/stream/datetime.cpp \
        $$PWD/src/stream/geometry.cpp

    HEADERS += \
        $$PWD/src/private/qt_types_p.h \
        $$PWD/src/stream/datetime.h \
        $$PWD/src/stream/geometry.h

    qtHaveModule(gui) {
        QT += gui
    }

    qtHaveModule(location) {
        QT += location

        SOURCES += $$PWD/src/stream/location.cpp
        HEADERS += $$PWD/src/stream/location.h
    }
}
