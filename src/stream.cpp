#include "stream.h"
#include <QBuffer>
#include "private/sysdep.h"
#include "private/pack_p.h"
#include "msgpack_common.h"
#include <limits>
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
    dev(0), owndev(false), q_status(Ok)
{ }

MsgPackStream::MsgPackStream(QIODevice *d) :
    dev(d), owndev(false)
{ }

MsgPackStream::MsgPackStream(QByteArray *a, QIODevice::OpenMode mode) :
    owndev(true), q_status(Ok)
{
    QBuffer *buf = new QBuffer(a);
    buf->open(mode);
    dev = buf;
}

MsgPackStream::MsgPackStream(const QByteArray &a) :
    owndev(true), q_status(Ok)
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
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_quint8(u8, &p))
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint16 &u16)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_quint16(u16, &p {
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint32 &u32)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_quint32(u32, &p {
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint64 &u64)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_quint64(u64, &p {
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint8 &i8)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_qint8(i8, &p))         setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint16 &i16)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_qint16(i16, &p){
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint32 &i32)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_qint32(i32, &p){
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint64 &i64)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p;
    if (dev->read((char *)&p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (!unpack_upto_qint64(i64, &p){
        setStatus(ReadCorruptData);
    return *this;
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
    quint8 *data = new quint8[len];
    if (dev->read((char *)data, len) != len) {
        setStatus(ReadPastEnd);
        delete[] data;
        return *this;
    }
    str = QString::fromUtf8((const char*) data, len);
    delete[] data;
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
    if (dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(quint64 u64)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[9];
    quint8 sz = MsgPackPrivate::pack_ulonglong(u64, p, true) - p;
    if (dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(qint32 i32)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[5];
    quint8 sz = MsgPackPrivate::pack_int(i32, p, true) - p;
    if (dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(qint64 i64)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[9];
    quint8 sz = MsgPackPrivate::pack_longlong(i64, p, true) - p;
    if (dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(QString str)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 *p = (quint8 *)0;
    quint32 sz = MsgPackPrivate::pack_string(str, p, false) - p;
    quint8 *data = new quint8[sz];
    MsgPackPrivate::pack_string(str, data, true);
    if (dev->write((char *)data, sz) != sz)
        setStatus(WriteFailed);
    delete[] data;
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(const char *str)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 *p = (quint8 *)0;
    quint32 str_len = strlen(str);
    quint32 sz = MsgPackPrivate::pack_string_raw(str, str_len, p, false) - p;
    quint8 *data = new quint8[sz];
    MsgPackPrivate::pack_string_raw(str, str_len, data, true);
    if (dev->write((char *)data, sz) != sz)
        setStatus(WriteFailed);
    delete[] data;
    return *this;
}

bool MsgPackStream::unpack_upto_quint8(quint8 &u8, quint8 *p)
{
    if (*p <= MsgPack::FirstByte::POSITIVE_FIXINT) {
        u8 = *p;
    } else if (*p == MsgPack::FirstByte::UINT8) {
        if (dev->read((char* )&u8, 1) != 1)
            return false;
    } else {
        return false;
    }
    return true;
}

bool MsgPackStream::unpack_upto_quint16(quint16 &u16, quint8 *p)
{
    if (*p == MsgPack::FirstByte::UINT16) {
        quint8 d[2];
        if (dev->read((char *)d, 2) != 2)
            return false;
        u16 = _msgpack_load16(quint16, d);
    } else {
        quint8 u8;
        bool ok = unpack_upto_quint8(u8, p);
        u16 = u8;
        return ok;
    }
    return true;
}

bool MsgPackStream::unpack_upto_quint32(quint32 &u32, quint8 *p)
{
    if (*p == MsgPack::FirstByte::UINT32) {
        quint8 d[4];
        if (dev->read((char *)d, 4) != 4)
            return false;
        u32 = _msgpack_load32(quint32, d);
        return true;
    } else {
        quint16 u16;
        bool ok = unpack_upto_quint16(u16, p);
        u32 = u16;
        return ok;
    }
    return true;
}

bool MsgPackStream::unpack_upto_quint64(quint64 &u64, quint8 *p)
{
    if (*p == MsgPack::FirstByte::UINT64) {
        quint8 d[8];
        if (dev->read((char *)d, 8) != 8) {
            return false;
        }
        u64 = _msgpack_load64(quint64, d);
        return true;
    } else {
        quint32 u32;
        bool ok = unpack_upto_quint32(u32, p);
        u64 = u32;
        return ok;
    }
}

bool MsgPackStream::unpack_upto_qint8(qint8 &i8, quint8 *p)
{
    if (*p >= MsgPack::FirstByte::NEGATIVE_FIXINT) {
        i8 = *p;
    } else if (*p == MsgPack::FirstByte::INT8) {
        if (dev->read((char *)&i8, 1) != 1) {
            return false;
        }
    } else {
        quint8 u8;
        bool ok = unpack_upto_quint8(u8, p);
        if (u8 > std::numeric_limits<qint8>::max())
            return false;
        else
            i8 = u8;
        return ok;
    }
    return true;
}

bool MsgPackStream::unpack_upto_qint16(qint16 &i16, quint8 *p)
{
    if (*p == MsgPack::FirstByte::INT16) {
        quint8 d[2];
        if (dev->read((char *)d, 2) != 2) {
            return false;
        }
        i16 = _msgpack_load16(qint16, d);     
    } else {
        qint8 i8;
        bool ok = unpack_upto_qint8(i8, p);
        if (ok) {
            i16 = i8;
        } else {
            quint16 u16;
            bool ok = unpack_upto_quint16(u16, p);
            if (u16 > std::numeric_limits<qint16>::max())
                return false;
            else
                i16 = u16;
            return ok;
        }
    }
    return true;
}

bool MsgPackStream::unpack_upto_qint32(qint32 &i32, quint8 *p)
{
    if(*p == MsgPack::FirstByte::INT32) {
        quint8 d[4];
        if (dev->read((char *)d, 4) != 4) {
            return false;
        }
        i32 = _msgpack_load32(qint32, d);
    } else {
        qint16 i16;
        bool ok = unpack_upto_qint16(i16, p);
        if (ok) {
            i32 = i16;
        } else {
            quint32 u32;
            bool ok = unpack_upto_quint32(u32, p);
            if (u32 > std::numeric_limits<qint32>::max())
                return false;
            else
                i32 = u32;
            return ok;
        }
    }
    return true;
}

bool MsgPackStream::unpack_upto_qint64(qint64 &i64, quint8 *p)
{
    if(*p == MsgPack::FirstByte::INT64) {
        quint8 d[8];
        if (dev->read((char *)d, 8) != 8)
            return false;
        i64 = _msgpack_load64(qint64, d);
        return true;
    } else {
        qint32 i32;
        bool ok = unpack_upto_qint32(i32, p);
        if (ok) {
            i64 = i32;
        } else {
            quint64 u64;
            bool ok = unpack_upto_quint64(u64, p);
            if (u64 > std::numeric_limits<qint64>::max())
                return false;
            else
                i64 = u64;
            return ok;
        }
    }
    return true;
}
