#ifndef STREAM_H
#define STREAM_H
#include <QIODevice>

class MsgPackStream
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

    void setCompatibility(bool isEnabled);

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
    MsgPackStream &operator>>(QVariantList &list);
    MsgPackStream &operator>>(QVariantMap &map);

    MsgPackStream &operator<<(bool b);
    MsgPackStream &operator<<(quint32 u32);
    MsgPackStream &operator<<(quint64 u64);
    MsgPackStream &operator<<(qint32 i32);
    MsgPackStream &operator<<(qint64 i64);
    MsgPackStream &operator<<(float f);
    MsgPackStream &operator<<(double d);
    MsgPackStream &operator<<(QString str);

    MsgPackStream &operator<<(QByteArray array);
    MsgPackStream &operator<<(QVariantList list);
    MsgPackStream &operator<<(QVariantMap map);

private:
    QIODevice *dev;
    bool compatibility;
    bool owndev;
    Status q_status;

    MsgPackStream &operator<<(const char *str); // use QStringLiteral instead

    bool unpack_upto_quint8(quint8 &u8, quint8 *p);
    bool unpack_upto_quint16(quint16 &u16, quint8 *p);
    bool unpack_upto_quint32(quint32 &u32, quint8 *p);
    bool unpack_upto_quint64(quint64 &u64, quint8 *p);
    bool unpack_upto_qint8(qint8 &i8, quint8 *p);
    bool unpack_upto_qint16(qint16 &i16, quint8 *p);
    bool unpack_upto_qint32(qint32 &i32, quint8 *p);
    bool unpack_upto_qint64(qint64 &i64, quint8 *p);
};

#endif // STREAM_H
