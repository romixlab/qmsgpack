#include "stream.h"
#include <QBuffer>
#include "private/sysdep.h"
#include "private/pack_p.h"
#include "msgpack_common.h"
#include <QDebug>

#undef  CHECK_STREAM_PRECOND
#ifndef QT_NO_DEBUG
#define CHECK_STREAM_PRECOND(retVal) \
    if (!dev) { \
        qWarning("msgpack::Stream: No device"); \
        return retVal; \
    }
#else
#define CHECK_STREAM_PRECOND(retVal) \
    if (!dev) { \
        return retVal; \
    }
#endif

#define CHECK_STREAM_WRITE_PRECOND(retVal) \
    CHECK_STREAM_PRECOND(retVal) \
    if (q_status != Ok) \
        return retVal;

MsgPackStream::MsgPackStream() :
    dev(0), compatibility(false), owndev(false), q_status(Ok)
{ }

MsgPackStream::MsgPackStream(QIODevice *d) :
    dev(d), compatibility(false), owndev(false)
{ }

MsgPackStream::MsgPackStream(QByteArray *a, QIODevice::OpenMode mode) :
    compatibility(false), owndev(true), q_status(Ok)
{
    QBuffer *buf = new QBuffer(a);
    buf->open(mode);
    dev = buf;
}

MsgPackStream::MsgPackStream(const QByteArray &a) :
    compatibility(false), owndev(true), q_status(Ok)
{
    QBuffer *buf = new QBuffer();
    buf->setData(a);
    buf->open(QIODevice::ReadOnly);
    dev = buf;
}

MsgPackStream::~MsgPackStream()
{
    if (owndev)
        delete dev;
}

void MsgPackStream::setDevice(QIODevice *d)
{
    if (owndev)
        delete dev;
    dev = d;
    owndev = false;
}

bool MsgPackStream::atEnd() const
{
    return dev ? dev->atEnd() : true;
}

void MsgPackStream::setCompatibility(bool isEnabled)
{
    compatibility = isEnabled;
}

MsgPackStream::Status MsgPackStream::status() const
{
    return q_status;
}

void MsgPackStream::resetStatus()
{
    q_status = Ok;
}

void MsgPackStream::setStatus(Status status)
{
    q_status = status;
}

MsgPackStream &MsgPackStream::operator>>(bool &b)
{
    CHECK_STREAM_PRECOND(*this)
    quint8 p[1];
    if (!dev->getChar((char *)p)) {
        b = false;
        setStatus(ReadPastEnd);
    } else {
        if (p[0] != MsgPack::FirstByte::TRUE ||
            p[0] != MsgPack::FirstByte::FALSE)
            setStatus(ReadCorruptData);
        b = (p[0] == MsgPack::FirstByte::TRUE);
    }
    return *this;
}

MsgPackStream &MsgPackStream::operator >>(quint8 &u8)
{
    CHECK_STREAM_PRECOND(*this)
    char c;
    if (!dev->getChar(&c)) {
        u8 = 0;
        setStatus(ReadPastEnd);
    } else {
        u8 = quint8(c);
    }
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint16 &u16)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p[3];
    if (dev->read((char *)p, 3) != 3) {
        u16 = 0;
        setStatus(ReadPastEnd);
    } else {
        if (p[0] != MsgPack::FirstByte::UINT16)
            setStatus(ReadCorruptData);
        u16 = _msgpack_load16(quint16, (p + 1));
    }
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint32 &u32)
{

}

MsgPackStream &MsgPackStream::operator>>(QString &str)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p[5];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }

    int len = 0;
    if (*p >= 0xa0 && *p <= 0xbf) { // fixstr
        len = (*p) & 0x1f; // 0b00011111
    } else if (*p == MsgPack::FirstByte::STR8) {
        if (dev->read((char *)p + 1, 1) == 1)
            len = p[1];
    } else if (*p == MsgPack::FirstByte::STR16) {
        if (dev->read((char *)p + 1, 2) == 2)
            len = _msgpack_load16(int, &p[1]);
    } else if (*p == MsgPack::FirstByte::STR32) {
        if (dev->read((char *)p + 1, 4) == 4)
            len = _msgpack_load32(int, &p[1]);
    } else {
        setStatus(ReadCorruptData);
        return *this;
    }
}

MsgPackStream &MsgPackStream::operator<<(bool b)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 m = b == true ?
                MsgPack::FirstByte::TRUE : MsgPack::FirstByte::FALSE;
    if (dev->write((char *)&m, 1) != 1)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(quint32 u32)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[5];
    quint8 sz = MsgPackPrivate::pack_uint(u32, p, true) - p;
    if (!dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(qint32 i32)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[5];
    quint8 sz = MsgPackPrivate::pack_uint(i32, p, true) - p;
    if (!dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(QString str)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 *p = (quint8 *)0;
    quint32 sz = MsgPackPrivate::pack_string(str, p, false) - p;
    quint8 *data = new quint8[sz];
    MsgPackPrivate::pack(str, data, true);
    if (dev->write((char *)data, sz) != sz)
        setStatus(WriteFailed);
    delete[] data;
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(const char *str)
{

}
