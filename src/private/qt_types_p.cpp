#include "qt_types_p.h"
#include "pack_p.h"
#include "unpack_p.h"
#include "sysdep.h"

#include <QDebug>

#ifdef QT_GUI_LIB
#include <QColor>
#endif
#define NO_QTGUI_WARNING "Library built without QtGui, hence some types are not available"

#include <QTime>
#include <QPoint>

bool MsgPackPrivate::register_qtype(QMetaType::Type q_type, quint8 msgpack_type)
{
	if (q_type == QMetaType::QColor) {
		#ifdef QT_GUI_LIB
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qcolor);
    	MsgPackPrivate::register_unpacker(msgpack_type, unpack_qcolor);
    	#else
    	qWarning() << NO_QTGUI_WARNING;
    	return false;
    	#endif //QT_GUI_LIB
    } else if (q_type == QMetaType::QTime) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qtime);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qtime);
    } else if (q_type == QMetaType::QDate) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qdate);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qdate);
    } else if (q_type == QMetaType::QDateTime) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qdatetime);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qdatetime);
    }
    return true;
}

#ifdef QT_GUI_LIB
quint32 MsgPackPrivate::pack_qcolor(const QVariant &variant, QByteArray &data, bool write)
{
    if (write) {
        QColor color = variant.value<QColor>();
        data.resize(4);
        data[0] = color.red();
        data[1] = color.green();
        data[2] = color.blue();
        data[3] = color.alpha();
    }
    return 4; // 4 bytes: r,g,b,a
}

QVariant MsgPackPrivate::unpack_qcolor(const QByteArray &data)
{
    return QColor((quint8)data[0], (quint8)data[1],
                  (quint8)data[2], (quint8)data[3]);
}
#endif //MsgPackPrivate

// Date and Time
void MsgPackPrivate::pack_qtime_raw(const QTime &time, quint8 *p)
{
    quint32 t = time.msecsSinceStartOfDay();
    _msgpack_store32(p, t);
}

QTime MsgPackPrivate::unpack_qtime_raw(quint8 *p)
{
    quint32 t = _msgpack_load32(quint32, p);
    return QTime::fromMSecsSinceStartOfDay(t);
}

quint32 MsgPackPrivate::pack_qtime(const QVariant &variant, QByteArray &data, bool write)
{
    if (write)
        pack_qtime_raw(variant.toTime(), (quint8 *)data.data());
    return 4; // 24h*60m*60s*1000ms = max 27 bits
}

QVariant MsgPackPrivate::unpack_qtime(const QByteArray &data)
{
    return unpack_qtime_raw((quint8 *)data.data());
}

void MsgPackPrivate::pack_qdate_raw(const QDate &date, quint8 *p)
{
    quint16 year = date.year();
    quint8 month = date.month();
    quint8 day = date.day();
    quint16 dy = year | (day & 0x80);
    quint8 md = (month << 4) | (day & 0xf);
    _msgpack_store16(p, dy);
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

quint32 MsgPackPrivate::pack_qdate(const QVariant &variant, QByteArray &data, bool write)
{
    if (write)
        pack_qdate_raw(variant.toDate(), (quint8 *)data.data());
    return 3; // d(5th bit)xyyyyyyyyyyyyyy, mmmmdddd
}

QVariant MsgPackPrivate::unpack_qdate(const QByteArray &data)
{
    return unpack_qdate_raw((quint8 *)data.data());
}

quint32 MsgPackPrivate::pack_qdatetime(const QVariant &variant, QByteArray &data, bool write)
{
    if (write) {
        quint8 *p = (quint8 *)data.data();
        QDateTime dt = variant.toDateTime();
        pack_qdate_raw(dt.date(), p);
        pack_qtime_raw(dt.time(), p + 3);
    }
    return 7; // 3 for date, 4 for time
}

QVariant MsgPackPrivate::unpack_qdatetime(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    QDate d = unpack_qdate_raw(p);
    QTime t = unpack_qtime_raw(p + 3);
    return QDateTime(d, t);
}

// Points and Vectors
quint32 MsgPackPrivate::pack_qpoint(const QVariant &variant, QByteArray &data, bool write)
{
    QPoint pt = variant.toPoint();
    quint8 *p = 0;
    if (write) {
        p = (quint8 *)data.data();
        p = MsgPackPrivate::pack_int(pt.x(), p, false);
        p = MsgPackPrivate::pack_int(pt.y(), p, false);
        return data.size(); // ok since it already resized
    } else {
        p = MsgPackPrivate::pack_int(pt.x(), p, false);
        p = MsgPackPrivate::pack_int(pt.y(), p, false);
        return p - (quint8 *)0;
    }
}

QVariant MsgPackPrivate::unpack_qpoint(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    QVariant v;
    p = (MsgPackPrivate::unpackers[*p - 0xc0])(v, p);
    
}
