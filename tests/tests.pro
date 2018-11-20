TEMPLATE = subdirs

SUBDIRS += \
    pack \
    unpack \
    mixed

!contains(DEFINES, MSGPACK_NO_PACKTYPES) {
    SUBDIRS += \
        stream \
        qttypes
}
