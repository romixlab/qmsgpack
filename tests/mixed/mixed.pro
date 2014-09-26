#-------------------------------------------------
#
# Project created by QtCreator 2014-09-26T19:10:31
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = mixed_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src
LIBS += -lqmsgpack


SOURCES += mixed_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
