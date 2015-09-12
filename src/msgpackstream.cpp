#include "msgpackstream.h"
#include "private/pack_p.h"

#include <QBuffer>
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

QIODevice *MsgPackStream::device() const
{
    return dev;
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
        if (p[0] != MsgPack::FirstByte::MTRUE ||
            p[0] != MsgPack::FirstByte::MFALSE)
            setStatus(ReadCorruptData);
        b = (p[0] == MsgPack::FirstByte::MTRUE);
    }
    return *this;
}

MsgPackStream &MsgPackStream::operator >>(quint8 &u8)
{
    CHECK_STREAM_PRECOND(*this);
    quint64 u64;
    if (unpack_ulonglong(u64) && u64 <= std::numeric_limits<quint8>::max())
        u8 = u64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint16 &u16)
{
    CHECK_STREAM_PRECOND(*this);
    quint64 u64;
    if (unpack_ulonglong(u64) && u64 <= std::numeric_limits<quint16>::max())
        u16 = u64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint32 &u32)
{
    CHECK_STREAM_PRECOND(*this);
    quint64 u64;
    if (unpack_ulonglong(u64) && u64 <= std::numeric_limits<quint32>::max())
        u32 = u64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(quint64 &u64)
{
    CHECK_STREAM_PRECOND(*this);
    unpack_ulonglong(u64);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint8 &i8)
{
    CHECK_STREAM_PRECOND(*this);
    qint64 i64;
    if (!unpack_longlong(i64))
        return *this;
    if (i64 >= std::numeric_limits<qint8>::min() &&
        i64 <= std::numeric_limits<qint8>::max())
        i8 = i64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint16 &i16)
{
    CHECK_STREAM_PRECOND(*this);
qint64 i64;
    if (!unpack_longlong(i64))
        return *this;
    if (i64 >= std::numeric_limits<qint16>::min() &&
        i64 <= std::numeric_limits<qint16>::max())
        i16 = i64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint32 &i32)
{
    CHECK_STREAM_PRECOND(*this);
    qint64 i64;
    if (!unpack_longlong(i64))
        return *this;
    if (i64 >= std::numeric_limits<qint32>::min() &&
        i64 <= std::numeric_limits<qint32>::max())
        i32 = i64;
    else
        setStatus(ReadCorruptData);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(qint64 &i64)
{
    CHECK_STREAM_PRECOND(*this);
    unpack_longlong(i64);
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(float &f)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 *fp = (quint8 *)&f;
    quint8 p[5];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (p[0] != MsgPack::FirstByte::FLOAT32) {
        setStatus(ReadCorruptData);
        return *this;
    }
    if (dev->read((char *)p + 1, 4) != 4) {
        setStatus(ReadPastEnd);
        return *this;
    }
#ifdef __LITTLE_ENDIAN__
    for (int i = 0; i < 4; ++i)
        *(fp + 3 - i) = *(p + i + 1);
#else
    for (int i = 0; i < 4; ++i)
        *(fp + i) = *(p + i + 1);
#endif
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(double &d)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 *fp = (quint8 *)&d;
    quint8 p[9];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    if (p[0] != MsgPack::FirstByte::FLOAT64) {
        setStatus(ReadCorruptData);
        return *this;
    }
    if (dev->read((char *)p + 1, 8) != 8) {
        setStatus(ReadPastEnd);
        return *this;
    }
#ifdef __LITTLE_ENDIAN__
    for (int i = 0; i < 8; ++i)
        *(fp + 7 - i) = *(p + i + 1);
#else
    for (int i = 0; i < 8; ++i)
        *(fp + i) = *(p + i + 1);
#endif
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
    return *this;
}

MsgPackStream &MsgPackStream::operator>>(QByteArray &array)
{
    CHECK_STREAM_PRECOND(*this);
    quint8 p[5];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return *this;
    }
    quint32 len;
    if (p[0] == MsgPack::FirstByte::BIN8) {
        if (dev->read((char *)p + 1, 1) != 1) {
            setStatus(ReadPastEnd);
            return *this;
        }
        len = p[1];
    } else if (p[0] == MsgPack::FirstByte::BIN16) {
        if (dev->read((char *)p + 1, 2) != 2) {
            setStatus(ReadPastEnd);
            return *this;
        }
        len = _msgpack_load16(quint16, &p[1]);
    } else if (p[0] == MsgPack::FirstByte::BIN32) {
        if (dev->read((char *)p + 1, 4) != 4) {
            setStatus(ReadPastEnd);
            return *this;
        }
        len = _msgpack_load32(quint32, &p[1]);
    } else {
        setStatus(ReadCorruptData);
        return *this;
    }
    array.resize(len);
    if (dev->read(array.data(), len) != len)
        setStatus(ReadPastEnd);
    return *this;
}

bool MsgPackStream::readBytes(char *data, uint len)
{
    CHECK_STREAM_PRECOND(false);
    uint readed = dev->read(data, len);
    if (readed != len) {
        setStatus(ReadPastEnd);
        return false;
    }
    return true;
}

bool MsgPackStream::readExtHeader(quint32 &len)
{
    CHECK_STREAM_PRECOND(false);
    quint8 d[6];
    if (dev->read((char*)d, 2) != 2) {
        setStatus(ReadPastEnd);
        return false;
    }
    if (d[0] >= MsgPack::FirstByte::FIXEXT1 &&
            d[0] <= MsgPack::FirstByte::FIXEX16) {
        len = 1;
        len <<= d[0] - MsgPack::FirstByte::FIXEXT1;
        return true;
    }

    quint8 toRead = 1;
    toRead <<= d[0] - MsgPack::FirstByte::EXT8;
    if (dev->read((char*)&d[2], toRead) != toRead) {
        setStatus(ReadPastEnd);
        return false;
    }

    if (d[0] == MsgPack::FirstByte::EXT8) {
        len = d[1];
    } else if (d[0] == MsgPack::FirstByte::EXT16) {
        len = _msgpack_load16(quint32, &d[1]);
    } else if (d[0] == MsgPack::FirstByte::EXT32) {
        len = _msgpack_load32(quint32, &d[1]);
    } else {
        setStatus(ReadCorruptData);
        return false;
    }
    return true;
}

MsgPackStream &MsgPackStream::operator<<(bool b)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 m = b == true ?
                MsgPack::FirstByte::MTRUE : MsgPack::FirstByte::MFALSE;
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

MsgPackStream &MsgPackStream::operator<<(float f)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[5];
    quint8 sz = MsgPackPrivate::pack_float(f, p, true) - p;
    if (dev->write((char *)p, sz) != sz)
        setStatus(WriteFailed);
    return *this;
}

MsgPackStream &MsgPackStream::operator<<(double d)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[9];
    quint8 sz = MsgPackPrivate::pack_double(d, p, true) - p;
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

MsgPackStream &MsgPackStream::operator<<(QByteArray array)
{
    CHECK_STREAM_WRITE_PRECOND(*this);
    quint8 p[5];
    quint32 len = array.length();
    quint8 header_len = MsgPackPrivate::pack_bin_header(len, p, true) - p;
    if (dev->write((char *)p, header_len) != header_len) {
        setStatus(WriteFailed);
        return *this;
    }
    if (dev->write(array.data(), len) != len)
        setStatus(WriteFailed);
    return *this;
}

bool MsgPackStream::writeBytes(const char *data, uint len)
{
    CHECK_STREAM_WRITE_PRECOND(false);
    if (dev->write(data, len) != len) {
        setStatus(WriteFailed);
        return false;
    }
    return true;
}

bool MsgPackStream::writeExtHeader(quint32 len, qint8 msgpackType)
{
    CHECK_STREAM_WRITE_PRECOND(false);
    quint8 d[6];
    d[1] = msgpackType;
    quint8 sz = 2;
    if (len == 1) {
        d[0] = 0xd4;
    } else if (len == 2) {
        d[0] = 0xd5;
    } else if (len == 4) {
        d[0] = 0xd6;
    } else if (len == 8) {
        d[0] = 0xd7;
    } else if (len == 16) {
        d[0] = 0xd8;
    } else if (len <= std::numeric_limits<quint8>::max()) {
        d[0] = 0xc7;
        d[1] = (quint8)len;
        d[2] = msgpackType;
        sz = 3;
    } else if (len <= std::numeric_limits<quint16>::max()) {
        d[0] = 0xc8;
        _msgpack_store16(&d[1], len);
        d[3] = msgpackType;
        sz = 4;
    } else {
        d[0] = 0xc9;
        _msgpack_store32(&d[1], len);
        d[5] = msgpackType;
        sz = 6;
    }
    if (dev->write((const char *)d, sz) != sz) {
        setStatus(WriteFailed);
        return false;
    }
    return true;
}

bool MsgPackStream::unpack_longlong(qint64 &i64)
{
    quint8 p[9];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return false;
    }

    if (p[0] <= 127) {// positive fixint 0x00 - 0x7f
        i64 = p[0];
        return true;
    } else if (*p >= 0xe0) { // negative fixint 0xe0 - 0xff
        i64 = (qint8)p[0];
        return true;
    }

    static int typeLengths[] = {1, 2, 4, 8, 1, 2, 4, 8};
    int typeLength = typeLengths[p[0] - MsgPack::FirstByte::UINT8];
    if (dev->read((char *)p + 1, typeLength) != typeLength) {
        setStatus(ReadPastEnd);
        return false;
    }
    if (p[0] == MsgPack::FirstByte::UINT8) {
        i64 = p[1];
    } else if (p[0] == MsgPack::FirstByte::INT8) {
        i64 = (qint8)p[1];
    } else if (p[0] == MsgPack::FirstByte::UINT16) {
        i64 = _msgpack_load16(quint16, p + 1);
    } else if (p[0] == MsgPack::FirstByte::INT16) {
        i64 = _msgpack_load16(qint16, p + 1);
    } else if (p[0] == MsgPack::FirstByte::UINT32) {
        i64 = _msgpack_load32(quint32, p + 1);
    } else if (p[0] == MsgPack::FirstByte::INT32) {
        i64 = _msgpack_load32(qint32, p + 1);
    } else if (p[0] == MsgPack::FirstByte::UINT64) {
        quint64 u64;
        u64 = _msgpack_load64(quint64, p + 1);
        if (u64 > (quint64)std::numeric_limits<qint64>::max()) {
            setStatus(ReadCorruptData);
            return false;
        }
        i64 = u64;
    } else if (p[0] == MsgPack::FirstByte::INT64) {
        i64 = _msgpack_load64(qint64, p + 1);
    }
    return true;
}

bool MsgPackStream::unpack_ulonglong(quint64 &u64)
{
    quint8 p[9];
    if (dev->read((char *)p, 1) != 1) {
        setStatus(ReadPastEnd);
        return false;
    }

    if (p[0] <= 127) {// positive fixint 0x00 - 0x7f
        u64 = p[0];
        return true;
    } else if (*p >= 0xe0) { // negative fixint 0xe0 - 0xff
        return false;
    }

    static int typeLengths[] = {1, 2, 4, 8, 1, 2, 4, 8};
    int typeLength = typeLengths[p[0] - MsgPack::FirstByte::UINT8];
    if (dev->read((char *)p + 1, typeLength) != typeLength) {
        setStatus(ReadPastEnd);
        return false;
    }
    if (p[0] == MsgPack::FirstByte::UINT8) {
        u64 = p[1];
        return true;
    } else if (p[0] == MsgPack::FirstByte::UINT16) {
        u64 = _msgpack_load16(quint64, p + 1);
        return true;
    } else if (p[0] == MsgPack::FirstByte::UINT32) {
        u64 = _msgpack_load32(quint64, p + 1); 
        return true;       
    } else if (p[0] == MsgPack::FirstByte::UINT64) {
        u64 = _msgpack_load64(quint64, p + 1);
        return true;
    }
    setStatus(ReadCorruptData);
    return false;
}
