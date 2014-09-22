#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T17:43:05
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qmsgpack
CONFIG   -= app_bundle

TEMPLATE = lib
DEFINES += MSGPACK_MAKE_LIB
target.path = ../build
INSTALLS += target
QMAKE_CXXFLAGS += -fPIC


SOURCES += main.cpp \
    msgpack.cpp \
    private/pack_p.cpp \
    private/unpack_p.cpp

HEADERS += \
    msgpack.h \
    private/pack_p.h \
    private/unpack_p.h \
    private/sysdep.h \
    msgpack_common.h \
    msgpack_export.h
