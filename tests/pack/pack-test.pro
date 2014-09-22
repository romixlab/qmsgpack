#-------------------------------------------------
#
# Project created by QtCreator 2014-09-20T15:17:37
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = pack_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lqmsgpack
INCLUDEPATH += ../../src


SOURCES += pack_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
