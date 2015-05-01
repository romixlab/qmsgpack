#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T17:43:05
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = qmsgpack
CONFIG   -= app_bundle

TEMPLATE = lib
DEFINES += MSGPACK_MAKE_LIB
DESTDIR = $$_PRO_FILE_PWD_/../bin
INSTALLS += target
QMAKE_CXXFLAGS += -fPIC

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,d)
}


SOURCES += msgpack.cpp \
    msgpack_common.cpp \
    private/pack_p.cpp \
    private/unpack_p.cpp \
    private/qt_types_p.cpp \
    msgpack_ext.cpp \
    stream.cpp

HEADERS += \
    msgpack.h \
    private/pack_p.h \
    private/unpack_p.h \
    private/sysdep.h \
    msgpack_common.h \
    msgpack_export.h \
    private/qt_types_p.h \
    msgpack_ext.h \
    stream.h
