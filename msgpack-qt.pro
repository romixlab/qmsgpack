#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T17:43:05
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qmsgpack
#CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    msgpack.cpp \
    private/pack_p.cpp \
    private/unpack_p.cpp

HEADERS += \
    msgpack.h \
    private/pack_p.h \
    private/unpack_p.h
