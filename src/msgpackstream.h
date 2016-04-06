#ifndef STREAM_H
#define STREAM_H

#include "msgpack_export.h"
#include "msgpackcommon.h"
#include "endianhelper.h"

#include <limits>

#include <QIODevice>

class MSGPACK_EXPORT MsgPackStream
{
public:
    MsgPackStream();
    MsgPackStream(QIODevice *d);
    MsgPackStream(QByteArray *a, QIODevice::OpenMode mode);
    MsgPackStream(const QByteArray &a);
    virtual ~MsgPackStream();

    void setDevice(QIODevice *d);
    QIODevice *device() const;
    bool atEnd() const;

    enum Status {Ok, ReadPastEnd, ReadCorruptData, WriteFailed};
    Status status() const;
    void resetStatus();
    void setStatus(Status status);

    MsgPackStream &operator>>(bool &b);
    MsgPackStream &operator>>(quint8 &u8);
    MsgPackStream &operator>>(quint16 &u16);
    MsgPackStream &operator>>(quint32 &u32);
    MsgPackStream &operator>>(quint64 &u64);
    MsgPackStream &operator>>(qint8 &i8);
    MsgPackStream &operator>>(qint16 &i16);
    MsgPackStream &operator>>(qint32 &i32);
    MsgPackStream &operator>>(qint64 &i64);
    MsgPackStream &operator>>(float &f);
    MsgPackStream &operator>>(double &d);
    MsgPackStream &operator>>(QString &str);
    MsgPackStream &operator>>(QByteArray &array);
    bool readBytes(char *data, uint len);
    bool readExtHeader(quint32 &len);

    MsgPackStream &operator<<(bool b);
    MsgPackStream &operator<<(quint32 u32);
    MsgPackStream &operator<<(quint64 u64);
    MsgPackStream &operator<<(qint32 i32);
    MsgPackStream &operator<<(qint64 i64);
    MsgPackStream &operator<<(float f);
    MsgPackStream &operator<<(double d);
    MsgPackStream &operator<<(QString str);
    MsgPackStream &operator<<(const char *str);
    MsgPackStream &operator<<(QByteArray array);
    bool writeBytes(const char *data, uint len);
    bool writeExtHeader(quint32 len, qint8 msgpackType);

private:
    QIODevice *dev;
    bool owndev;
    Status q_status;

    bool unpack_longlong(qint64 &i64);
    bool unpack_ulonglong(quint64 &u64);
};

template <typename T>
MsgPackStream& operator<<(MsgPackStream& s, const QList<T> &list)
{
    quint32 len = list.size();
    quint8 p[5];
    if (len <= 15) {
        p[0] = MsgPack::FirstByte::FIXARRAY | len;
        s.writeBytes((const char *)p, 1);
    } else if (len <= std::numeric_limits<quint16>::max()) {
        p[0] = MsgPack::FirstByte::ARRAY16;
        _msgpack_store16(p + 1, len);
        s.writeBytes((const char *)p, 3);
    } else {
        p[0] = MsgPack::FirstByte::ARRAY32;
        _msgpack_store32(p + 1, len);
        s.writeBytes((const char *)p, 5);
    }
    if (s.status() != MsgPackStream::Ok)
        return s;
    for (int i = 0; i < list.size(); ++i)
        s << list[i];
    return s;
}

template <typename T>
MsgPackStream& operator>>(MsgPackStream& s, QList<T> &list)
{
    list.clear();
    quint8 p[5];
    quint32 len;
    s.readBytes((char *)p, 1);
    if (p[0] >= 0x90 && p[0] <= 0x9f) {
        len = p[0] & 0xf;
    } else if (p[0] == MsgPack::FirstByte::ARRAY16) {
        s.readBytes((char *)p + 1, 2);
        len = _msgpack_load16(quint16, p + 1);
    } else if (p[0] == MsgPack::FirstByte::ARRAY32) {
        s.readBytes((char *)p + 1, 4);
        len = _msgpack_load32(quint32, p + 1);
    }
    for (quint32 i = 0; i < len; ++i) {
        T t;
        s >> t;
        list.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

#endif // STREAM_H
