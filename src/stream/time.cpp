#include "time.h"
#include "../msgpack.h"

void MsgPackPrivate::pack_qtime_raw(const QTime &time, quint8 *p)
{
    quint8 hm, ms;
    hm = (quint8)time.hour() << 4;
    hm |= (quint8)time.minute() >> 2;
    ms = ((quint8)time.minute() << 6) & 0xc0; // 11000000
    ms |= (quint8)time.second();
    p[0] = hm; p[1] = ms;

    if (time.msec() != 0) {
        p[2] = (quint8)( (quint16)time.msec() >> 8 );
        p[3] = (quint8)time.msec();
    }
}

QTime MsgPackPrivate::unpack_qtime_raw(quint8 *p, bool with_ms)
{
    quint8 h, m, s;
    quint16 ms = 0;
    h = p[0] >> 4;
    m = (p[0] << 2) | (p[1] >> 6);
    m &= 0x3f; // 00111111
    s = p[1] & 0x3f;
    if (with_ms)
        ms = (p[2] << 8) | p[3];
    return QTime(h, m, s, ms);
}

void MsgPackPrivate::pack_qdate_raw(const QDate &date, quint8 *p)
{
    quint16 year = date.year();
    quint8 month = date.month();
    quint8 day = date.day();
    if (day > 15)
        year |= 0x8000;
    quint8 md = (month << 4) | (day & 0xf);
    _msgpack_store16(p, year);
    p[2] = md;
}

QDate MsgPackPrivate::unpack_qdate_raw(quint8 *p)
{
    quint16 year = _msgpack_load16(quint16, p);
    quint8 month = (p[2] & 0xf0) >> 4;
    quint8 day = p[2] & 0xf;
    day |= (quint8)((year & 0x8000) >> 11);
    year &= 0x7fff;
    return QDate(year, month, day);
}

MsgPackStream &operator<<(MsgPackStream &s, const QTime &t)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QTime);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    quint8 len = 1;
    if (t.isValid())
        len = t.msec() == 0 ? 2 : 4;
    s.writeExtHeader(len, msgpackType);
    quint8 p[4] = {0};
    if (len != 1)
        MsgPackPrivate::pack_qtime_raw(t, p);
    s.writeBytes((const char *)p, len);
    return s;
}

MsgPackStream& operator>>(MsgPackStream& s, QTime &t)
{
    quint32 len;
    s.readExtHeader(len);
    if (len != 1 && len != 2 && len != 4) {
        s.setStatus(MsgPackStream::ReadCorruptData);
        t = QTime();
        return s;
    }
    if (len == 1) {
        t == QTime();
        return s;
    }
    quint8 p[4];
    s.readBytes((char *)p, len);
    t = MsgPackPrivate::unpack_qtime_raw(p, len == 4);
    return s;
}

MsgPackStream &operator<<(MsgPackStream &s, const QDate &date)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QDate);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    quint8 len = date.isValid() ? 3 : 1;
    s.writeExtHeader(len, msgpackType);
    quint8 p[3] = {0};
    if (len != 1)
        MsgPackPrivate::pack_qdate_raw(date, p);
    s.writeBytes((const char *)p, len);
    return s;
}

MsgPackStream& operator>>(MsgPackStream& s, QDate &date)
{
    quint32 len;
    s.readExtHeader(len);
    if (len != 1 && len != 3) {
        s.setStatus(MsgPackStream::ReadCorruptData);
        date = QDate();
        return s;
    }
    if (len == 1) {
        date = QDate();
        return s;
    }
    quint8 p[3];
    s.readBytes((char *)p, len);
    date = MsgPackPrivate::unpack_qdate_raw(p);
    return s;
}

MsgPackStream &operator<<(MsgPackStream& s, const QDateTime &dt)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QDateTime);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    quint8 len = 1;
    if (dt.isValid()) {
        len = 3;
        len += dt.time().msec() == 0 ? 2 : 4;
    }
    s.writeExtHeader(len, msgpackType);
    quint8 p[7] = {0};
    if (len != 1) {
        MsgPackPrivate::pack_qdate_raw(dt.date(), p);
        MsgPackPrivate::pack_qtime_raw(dt.time(), p + 3);
    }
    s.writeBytes((const char *)p, len);
    return s;
}

MsgPackStream& operator>>(MsgPackStream& s, QDateTime &dt)
{
    quint32 len;
    s.readExtHeader(len);
    if (len != 1 && len != 5 && len != 7) {
        s.setStatus(MsgPackStream::ReadCorruptData);
        dt = QDateTime();
        return s;
    }
    if (len == 1) {
        dt = QDateTime();
        return s;
    }
    quint8 p[7];
    s.readBytes((char *)p, len);
    dt.setDate(MsgPackPrivate::unpack_qdate_raw(p));
    dt.setTime(MsgPackPrivate::unpack_qtime_raw(p + 3, len == 7));
    return s;
}
