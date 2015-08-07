TEMPLATE        =  lib
CONFIG          += shared
QMAKE_CXXFLAGS  += -std=c++11
TARGET          =  qmsgpack
DEFINES         += MSGPACK_MAKE_LIB
HEADERS         += msgpack_common.h
VERSION         =  0.1.0
message($${PWD})
message($$_PRO_FILE_PWD_)

HEADERS += \
    msgpack_common.h \
    msgpackstream.h \
    msgpack_export.h \
    msgpack.h \
    endianhelper.h \
    private/unpack_p.h \
    private/qt_types_p.h \
    private/pack_p.h

SOURCES += \
    msgpackstream.cpp \
    msgpack_common.cpp \
    msgpack.cpp \
    private/qt_types_p.cpp \
    private/unpack_p.cpp \
    private/pack_p.cpp
