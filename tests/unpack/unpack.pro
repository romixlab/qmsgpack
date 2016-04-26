#-------------------------------------------------
#
# Project created by QtCreator 2014-09-25T10:25:22
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = unpack_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
include($$PWD/../../qmsgpack.pri)
INCLUDEPATH += ../../src


SOURCES += unpack_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
