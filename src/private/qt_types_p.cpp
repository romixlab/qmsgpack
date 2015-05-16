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
#include <QRect>

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
    } else if (q_type == QMetaType::QPoint) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qpoint);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qpoint);
    } else if (q_type == QMetaType::QSize) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qsize);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qsize);
    } else if (q_type == QMetaType::QRect) {
        MsgPackPrivate::register_packer(q_type, msgpack_type, pack_qrect);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qrect);
    }
    return true;
}

#ifdef QT_GUI_LIB
QByteArray MsgPackPrivate::pack_qcolor(const QVariant &variant)
{
    QByteArray data;
    data.resize(4);
    QColor color = variant.value<QColor>();
    quint8 *p = (quint8 *)data.data();
    p[0] = color.red();
    p[1] = color.green();
    p[2] = color.blue();
    p[3] = color.alpha();
    return data;
}

QVariant MsgPackPrivate::unpack_qcolor(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    return QColor(p[0], p[1], p[2], p[3]);
}
#endif // QT_GUI_LIB

// Date and Time
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

QByteArray MsgPackPrivate::pack_qtime(const QVariant &variant)
{
    QTime time = variant.toTime();
    quint8 size = time.msec() == 0 ? 2 : 4;
    QByteArray data;
    data.resize(size);
    pack_qtime_raw(time, (quint8 *)data.data());
    return data;
}

QVariant MsgPackPrivate::unpack_qtime(const QByteArray &data)
{
    return unpack_qtime_raw((quint8 *)data.data(), data.size() == 4);
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

QByteArray MsgPackPrivate::pack_qdate(const QVariant &variant)
{
    QByteArray data;
    data.resize(3);
    pack_qdate_raw(variant.toDate(), (quint8 *)data.data());
    return data;
}

QVariant MsgPackPrivate::unpack_qdate(const QByteArray &data)
{
    return unpack_qdate_raw((quint8 *)data.data());
}

QByteArray MsgPackPrivate::pack_qdatetime(const QVariant &variant)
{
    QDateTime dt = variant.toDateTime();
    quint8 time_size = dt.time().msec() == 0 ? 2 : 4;
    QByteArray data;
    data.resize(3 + time_size);
    quint8 *p = (quint8 *)data.data();
    pack_qdate_raw(dt.date(), p);
    p += 3;
    pack_qtime_raw(dt.time(), p);
    return data;
}

QVariant MsgPackPrivate::unpack_qdatetime(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    QDate d = unpack_qdate_raw(p);
    QTime t = unpack_qtime_raw(p + 3, data.size() == 7);
    return QDateTime(d, t);
}

// Points and Vectors
QByteArray MsgPackPrivate::pack_qpoint(const QVariant &variant)
{
    // QPoint pt = variant.toPoint();
    // quint8 size = pack_two_integers(pt.x(), pt.y(), 0, false);
    // if (write) {
    //     data.resize(size);
    //     pack_two_integers(pt.x(), pt.y(), (quint8 *)data.data(), true);
    // }
    // return size;
}

QVariant MsgPackPrivate::unpack_qpoint(const QByteArray &data)
{
    // quint8 *p = (quint8 *)data.data();
    // qint32 x;
    // bool ok;
    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // QPoint pt;
    // pt.setX(x);
    // MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // pt.setY(x);
    // return pt;
}

QByteArray MsgPackPrivate::pack_qsize(const QVariant &variant)
{
    // QSize sz = variant.toSize();
    // quint8 size = pack_two_integers(sz.width(), sz.height(), 0, false);
    // if (write) {
    //     data.resize(size);
    //     pack_two_integers(sz.width(), sz.height(), (quint8 *)data.data(), true);
    // }
    // return size;
}

QVariant MsgPackPrivate::unpack_qsize(const QByteArray &data)
{
    // quint8 *p = (quint8 *)data.data();
    // qint32 x;
    // bool ok;
    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // QSize sz;
    // sz.setWidth(x);
    // MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // sz.setHeight(x);
    // return sz;
}

QByteArray MsgPackPrivate::pack_qrect(const QVariant &variant)
{
    // QRect rect = variant.toRect();
    // QPoint pt1 = rect.topLeft();
    // QPoint pt2 = rect.bottomRight();
    // quint8 size = pack_two_integers(pt1.x(), pt1.y(), 0, false);
    // size += pack_two_integers(pt2.x(), pt2.y(), 0, false);
    // if (write) {
    //     data.resize(size);
    //     quint8 *p = (quint8 *)data.data();
    //     p += pack_two_integers(pt1.x(), pt1.y(), p, true);
    //     pack_two_integers(pt2.x(), pt2.y(), p, true);
    // }
    // return size;
}

QVariant MsgPackPrivate::unpack_qrect(const QByteArray &data)
{
    // quint8 *p = (quint8 *)data.data();
    // qint32 x;
    // bool ok;

    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // QPoint pt;
    // pt.setX(x);
    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // pt.setY(x);

    // QRect rect;
    // rect.setTopLeft(pt);

    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // pt.setX(x);
    // p = MsgPack::Ext::unpack_upto_qint32(&x, p, &ok);
    // pt.setY(x);

    // rect.setBottomRight(pt);

    // return rect;
}

