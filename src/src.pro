TEMPLATE        =  lib
CONFIG          += shared
QMAKE_CXXFLAGS  += -std=c++11
TARGET          =  qmsgpack
DEFINES         += MSGPACK_MAKE_LIB
VERSION         =  0.1.0

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

release: DESTDIR = ../build/release
debug:   DESTDIR = ../build/debug
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc

HEADERS_INSTALL = \
    msgpack_common.h \
    msgpackstream.h \
    msgpack_export.h \
    msgpack.h \
    endianhelper.h

unix {
    header_files.files = $$HEADERS_INSTALL
    header_files.path = /usr/include/qmsgpack
    target.path = /usr/lib
    INSTALLS += header_files target
}

