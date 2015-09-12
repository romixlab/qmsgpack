#include "qt_types_p.h"
#include "pack_p.h"
#include "unpack_p.h"
#include "../msgpackstream.h"
#include "../endianhelper.h"
#include "../stream/time.h"

#include <QDebug>

#ifdef QT_GUI_LIB
#include <QColor>
#else
#define NO_QTGUI_WARNING "qmsgpack was built without QtGui, hence some types are not available"
#endif

#ifdef QT_LOCATION_LIB
#include <QGeoCoordinate>
#endif

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
#ifdef QT_LOCATION_LIB
    } else if ((int)q_type == qMetaTypeId<QGeoCoordinate>()) {
        MsgPackPrivate::register_packer((QMetaType::Type)qMetaTypeId<QGeoCoordinate>(),
                                        msgpack_type,
                                        pack_qgeocoordinate);
        MsgPackPrivate::register_unpacker(msgpack_type, unpack_qgeocoordinate);
#endif
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
    QColor color = variant.value<QColor>();
    if (!color.isValid())
        return QByteArray(1, 0);
    QByteArray data;
    data.resize(4);
    quint8 *p = (quint8 *)data.data();
    p[0] = color.red();
    p[1] = color.green();
    p[2] = color.blue();
    p[3] = color.alpha();
    return data;
}

QVariant MsgPackPrivate::unpack_qcolor(const QByteArray &data)
{
    if (data.length() == 1)
        return QColor();
    quint8 *p = (quint8 *)data.data();
    return QColor(p[0], p[1], p[2], p[3]);
}
#endif // QT_GUI_LIB

#ifdef QT_LOCATION_LIB
QByteArray MsgPackPrivate::pack_qgeocoordinate(const QVariant &variant)
{
    QGeoCoordinate c = variant.value<QGeoCoordinate>();
    if (!c.isValid())
        return QByteArray(1, 0);
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    out << c.latitude() << c.longitude();
    return ba;
}

QVariant MsgPackPrivate::unpack_qgeocoordinate(const QByteArray &data)
{
    if (data.length() == 1)
        return QVariant::fromValue(QGeoCoordinate());
    MsgPackStream in(data);
    QGeoCoordinate c;
    double x;
    in >> x;
    c.setLatitude(x);
    in >> x;
    c.setLongitude(x);
    return QVariant::fromValue(c);
}
#endif // QT_LOCATION_LIB

QByteArray MsgPackPrivate::pack_qtime(const QVariant &variant)
{
    QTime time = variant.toTime();
    if (!time.isValid())
        return QByteArray(1, 0);
    quint8 size = time.msec() == 0 ? 2 : 4;
    QByteArray data;
    data.resize(size);
    data.data()[0] = 0;
    pack_qtime_raw(time, (quint8 *)data.data());
    return data;
}

QVariant MsgPackPrivate::unpack_qtime(const QByteArray &data)
{
    if (data.size() == 1)
        return QTime();
    return unpack_qtime_raw((quint8 *)data.data(), data.size() == 4);
}

QByteArray MsgPackPrivate::pack_qdate(const QVariant &variant)
{
    QDate date = variant.toDate();
    if (!date.isValid())
        return QByteArray(1, 0);
    QByteArray data;
    data.resize(3);
    pack_qdate_raw(date, (quint8 *)data.data());
    return data;
}

QVariant MsgPackPrivate::unpack_qdate(const QByteArray &data)
{
    if (data.length() == 1)
        return QDate();
    return unpack_qdate_raw((quint8 *)data.data());
}

QByteArray MsgPackPrivate::pack_qdatetime(const QVariant &variant)
{
    QDateTime dt = variant.toDateTime();
    if (!dt.isValid())
        return QByteArray(1, 0);
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
    if (data.length() == 1)
        return QDateTime();
    quint8 *p = (quint8 *)data.data();
    QDate d = unpack_qdate_raw(p);
    QTime t = unpack_qtime_raw(p + 3, data.size() == 7);
    return QDateTime(d, t);
}

// Points and Vectors
QByteArray MsgPackPrivate::pack_qpoint(const QVariant &variant)
{
    QPoint pt = variant.toPoint();
    if (pt.isNull())
        return QByteArray(1, 0);
    QByteArray packed;
    MsgPackStream stream(&packed, QIODevice::WriteOnly);
    stream << pt.x() << pt.y();
    return packed;
}

QVariant MsgPackPrivate::unpack_qpoint(const QByteArray &data)
{
    if (data.length() == 1)
        return QPoint();
    MsgPackStream stream(data);
    qint32 x, y;
    stream >> x >> y;
    return QPoint(x, y);
}

QByteArray MsgPackPrivate::pack_qsize(const QVariant &variant)
{
    QSize sz = variant.toSize();
    if (!sz.isValid())
        return QByteArray(1, 0);
    QByteArray packed;
    MsgPackStream stream(&packed, QIODevice::WriteOnly);
    stream << sz.width() << sz.height();
    return packed;
}

QVariant MsgPackPrivate::unpack_qsize(const QByteArray &data)
{
    if (data.length() == 1)
        return QSize();
    MsgPackStream stream(data);
    qint32 width, height;
    stream >> width >> height;
    return QSize(width, height);
}

QByteArray MsgPackPrivate::pack_qrect(const QVariant &variant)
{
    QRect rect = variant.toRect();
    if (!rect.isValid())
        return QByteArray(1, 0);
    QPoint pt1 = rect.topLeft();
    QPoint pt2 = rect.bottomRight();
    QByteArray packed;
    MsgPackStream stream(&packed, QIODevice::WriteOnly);
    stream << pt1.x() << pt1.y() << pt2.x() << pt2.y();
    return packed;
}

QVariant MsgPackPrivate::unpack_qrect(const QByteArray &data)
{
    if (data.length() == 1)
        return QRect();
    MsgPackStream stream(data);
    qint32 x, y;
    stream >> x >> y;
    QRect rect;
    rect.setTopLeft(QPoint(x, y));
    stream >> x >> y;
    rect.setBottomRight(QPoint(x, y));
    return rect;
}
