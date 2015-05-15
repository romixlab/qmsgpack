#include "stream.h"
#include <QBuffer>
#include "private/sysdep.h"
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

