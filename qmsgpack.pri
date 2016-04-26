QT       += core location

DEFINES += MSGPACK_MAKE_LIB

INCLUDEPATH += $$PWD/src

SOURCES += \
    $$PWD/src/msgpack.cpp \
    $$PWD/src/msgpackcommon.cpp \
    $$PWD/src/private/pack_p.cpp \
    $$PWD/src/private/unpack_p.cpp \
    $$PWD/src/private/qt_types_p.cpp \
    $$PWD/src/msgpackstream.cpp \
    $$PWD/src/stream/time.cpp \
    $$PWD/src/stream/geometry.cpp \
    $$PWD/src/stream/location.cpp

HEADERS += \
    $$PWD/src/msgpack.h \
    $$PWD/src/private/pack_p.h \
    $$PWD/src/private/unpack_p.h \
    $$PWD/src/endianhelper.h \
    $$PWD/src/msgpackcommon.h \
    $$PWD/src/msgpack_export.h \
    $$PWD/src/private/qt_types_p.h \
    $$PWD/src/msgpackstream.h \
    $$PWD/src/stream/location.h \
    $$PWD/src/stream/time.h \
    $$PWD/src/stream/geometry.h
